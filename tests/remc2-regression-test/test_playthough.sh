
REMC2_EXECUTABLE=./remc2

echo "Running playthough demo levels 1-4"
ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=alignment=0 \
	${REMC2_EXECUTABLE} \
		--play_file ./Levels-1-4-Recording.bin\
		--config_file_path ./playthough-config.json\

if [ ${PIPESTATUS[1]} -eq 0 ]; then
	echo "Playthough success!"
else
	echo "Playthough failed!"
fi


