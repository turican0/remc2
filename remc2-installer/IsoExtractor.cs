using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace remc2_installer
{
	/// <summary>
	/// Extracts the files of an ISO 9660 CD image (e.g. game.gog of the GOG Magic Carpet Plus), without DOSBox.
	/// Reads plain 2048 byte sector images (.iso, .gog) and raw 2352 byte sector images (.bin, mode 1 and mode 2 form 1).
	/// </summary>
	public static class IsoExtractor
	{
		private const int LogicalSectorSize = 2048;
		private const int PrimaryVolumeDescriptorSector = 16;

		// The layout of the sectors in the image file
		private sealed class SectorLayout
		{
			public int RawSize;      // bytes of one sector in the file
			public int DataOffset;   // where the 2048 user data bytes start in it
		}

		/// <summary>
		/// Extracts the files of the image to the target directory, keeping the folder structure.
		/// </summary>
		/// <param name="imageFolder">A folder of the image (e.g. "CARPET") whose contents are extracted, null for the whole image.</param>
		/// <returns>The number of files extracted.</returns>
		public static int Extract(string imagePath, string targetDir, string imageFolder = null)
		{
			using (var image = new FileStream(imagePath, FileMode.Open, FileAccess.Read, FileShare.Read))
			{
				var layout = DetectLayout(image);
				if (layout == null)
					throw new InvalidDataException($"'{imagePath}' is not an ISO 9660 CD image");

				var pvd = ReadSectors(image, layout, PrimaryVolumeDescriptorSector, LogicalSectorSize);
				int blockSize = BitConverter.ToUInt16(pvd, 128);
				if (blockSize != LogicalSectorSize)
					throw new InvalidDataException($"Unsupported ISO 9660 block size {blockSize}");

				// the root directory record is at 156 of the primary volume descriptor
				var root = ParseRecord(pvd, 156);
				if (!string.IsNullOrEmpty(imageFolder))
				{
					foreach (var name in imageFolder.Split(new[] { '/', '\\' }, StringSplitOptions.RemoveEmptyEntries))
					{
						root = FindDirectory(image, layout, root, name)
							?? throw new DirectoryNotFoundException($"No folder '{imageFolder}' in the CD image '{imagePath}'");
					}
				}
				Directory.CreateDirectory(targetDir);
				return ExtractDirectory(image, layout, root.Extent, root.Length, targetDir, 0);
			}
		}

		// the sub folder of the directory with the name (case insensitive)
		private static DirectoryRecord? FindDirectory(Stream image, SectorLayout layout, DirectoryRecord directory, string name)
		{
			foreach (var record in ReadDirectory(image, layout, directory.Extent, directory.Length))
			{
				if (record.IsDirectory && string.Equals(record.Name, name, StringComparison.OrdinalIgnoreCase))
					return record;
			}
			return null;
		}

		private static SectorLayout DetectLayout(Stream image)
		{
			var candidates = new[]
			{
				new SectorLayout { RawSize = 2048, DataOffset = 0 },
				new SectorLayout { RawSize = 2352, DataOffset = 16 }, // mode 1: sync 12, header 4
				new SectorLayout { RawSize = 2352, DataOffset = 24 }, // mode 2 form 1: sync 12, header 4, subheader 8
			};
			var id = new byte[6];
			foreach (var layout in candidates)
			{
				long position = (long)PrimaryVolumeDescriptorSector * layout.RawSize + layout.DataOffset;
				if (position + id.Length > image.Length)
					continue;
				image.Position = position;
				ReadExactly(image, id, 0, id.Length);
				// volume descriptor type 1 (primary), standard identifier "CD001"
				if (id[0] == 1 && Encoding.ASCII.GetString(id, 1, 5) == "CD001")
					return layout;
			}
			return null;
		}

		private struct DirectoryRecord
		{
			public uint Extent;
			public uint Length;
			public bool IsDirectory;
			public string Name;
			public DateTime? Recorded;
		}

		private static DirectoryRecord ParseRecord(byte[] data, int offset)
		{
			int nameLength = data[offset + 32];
			var record = new DirectoryRecord
			{
				Extent = BitConverter.ToUInt32(data, offset + 2),
				Length = BitConverter.ToUInt32(data, offset + 10),
				IsDirectory = (data[offset + 25] & 0x02) != 0,
				Recorded = ParseDate(data, offset + 18),
			};
			if (nameLength == 1 && data[offset + 33] <= 1)
				record.Name = null; // 0 = the directory itself, 1 = its parent
			else
				record.Name = CleanName(Encoding.ASCII.GetString(data, offset + 33, nameLength), record.IsDirectory);
			return record;
		}

		private static int ExtractDirectory(Stream image, SectorLayout layout, uint extent, uint length, string targetDir, int depth)
		{
			if (depth > 32)
				throw new InvalidDataException("ISO 9660 directories are nested too deep");

			int count = 0;
			foreach (var record in ReadDirectory(image, layout, extent, length))
			{
				string path = Path.Combine(targetDir, record.Name);
				if (record.IsDirectory)
				{
					Directory.CreateDirectory(path);
					count += ExtractDirectory(image, layout, record.Extent, record.Length, path, depth + 1);
				}
				else
				{
					ExtractFile(image, layout, record.Extent, record.Length, path);
					if (record.Recorded.HasValue)
						File.SetLastWriteTime(path, record.Recorded.Value);
					count++;
				}
			}
			return count;
		}

		// the entries of a directory, without itself and its parent
		private static List<DirectoryRecord> ReadDirectory(Stream image, SectorLayout layout, uint extent, uint length)
		{
			var records = new List<DirectoryRecord>();
			var data = ReadSectors(image, layout, extent, (int)length);
			int offset = 0;
			while (offset < data.Length)
			{
				int recordLength = data[offset];
				if (recordLength == 0)
				{
					// records do not cross sectors: the rest of this sector is padding
					offset = (offset / LogicalSectorSize + 1) * LogicalSectorSize;
					continue;
				}
				if (offset + recordLength > data.Length || recordLength < 34)
					throw new InvalidDataException("Damaged ISO 9660 directory record");

				var record = ParseRecord(data, offset);
				offset += recordLength;
				if (record.Name != null)
					records.Add(record);
			}
			return records;
		}

		private static void ExtractFile(Stream image, SectorLayout layout, uint extent, uint length, string path)
		{
			using (var output = new FileStream(path, FileMode.Create, FileAccess.Write))
			{
				var buffer = new byte[LogicalSectorSize];
				uint remaining = length;
				long sector = extent;
				while (remaining > 0)
				{
					int chunk = (int)Math.Min(remaining, (uint)LogicalSectorSize);
					image.Position = sector * layout.RawSize + layout.DataOffset;
					ReadExactly(image, buffer, 0, chunk);
					output.Write(buffer, 0, chunk);
					remaining -= (uint)chunk;
					sector++;
				}
			}
		}

		// reads the user data of the sectors from the first one, length bytes
		private static byte[] ReadSectors(Stream image, SectorLayout layout, long firstSector, int length)
		{
			var data = new byte[length];
			int done = 0;
			long sector = firstSector;
			while (done < length)
			{
				int chunk = Math.Min(length - done, LogicalSectorSize);
				image.Position = sector * layout.RawSize + layout.DataOffset;
				ReadExactly(image, data, done, chunk);
				done += chunk;
				sector++;
			}
			return data;
		}

		private static void ReadExactly(Stream stream, byte[] buffer, int offset, int count)
		{
			while (count > 0)
			{
				int read = stream.Read(buffer, offset, count);
				if (read <= 0)
					throw new EndOfStreamException("The CD image ends inside a file");
				offset += read;
				count -= read;
			}
		}

		// "FONT0.DAT;1" -> "FONT0.DAT", "README.;1" -> "README"; the name may not leave the target folder
		private static string CleanName(string name, bool isDirectory)
		{
			if (!isDirectory)
			{
				int version = name.IndexOf(';');
				if (version >= 0)
					name = name.Substring(0, version);
				name = name.TrimEnd('.');
			}
			if (name.Length == 0 || name == "." || name == ".." || name.IndexOfAny(Path.GetInvalidFileNameChars()) >= 0)
				throw new InvalidDataException($"Invalid name in the CD image: '{name}'");
			return name;
		}

		// the 7 byte recording date: years since 1900, month, day, hour, minute, second, offset from GMT in 15 minutes
		private static DateTime? ParseDate(byte[] data, int offset)
		{
			try
			{
				var utc = new DateTime(1900 + data[offset], data[offset + 1], data[offset + 2],
					data[offset + 3], data[offset + 4], data[offset + 5], DateTimeKind.Utc);
				return utc.AddMinutes(-15 * (sbyte)data[offset + 6]).ToLocalTime();
			}
			catch (ArgumentOutOfRangeException)
			{
				return null;
			}
		}
	}
}
