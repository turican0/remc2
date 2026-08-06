$REMC2_EXECUTABLE_X86 = "$PSScriptRoot/../Release/remc2.exe"
$REMC2_EXECUTABLE_X64 = "$PSScriptRoot/../X64/Release/remc2.exe"
$CONFIG_FILE_PATH = "$PSScriptRoot/playthough-config.json"
$DEMO_FILE_PATH = "$PSScriptRoot/Levels-1-5-Recording.bin"

Write-Output "Running playthough demo levels 1-5 x86"
Write-Output $REMC2_EXECUTABLE
Write-Output $CONFIG_FILE_PATH
$process = Start-Process -Wait -PassThru -FilePath $REMC2_EXECUTABLE_X86 -ArgumentList "--play_file $DEMO_FILE_PATH --config_file_path $CONFIG_FILE_PATH"
$result = $process.ExitCode
	
if ($result -eq 0) {
    Write-Output "Playthough success!"
} else {
    Write-Output "Playthough failed!"
}

Read-Host -Prompt "Press ENTER to test x64 or CTRL+C to exit" | Out-Null

Write-Output "Running playthough demo levels 1-5 x64"
Write-Output $REMC2_EXECUTABLE
Write-Output $CONFIG_FILE_PATH
$process = Start-Process -Wait -PassThru -FilePath $REMC2_EXECUTABLE_X64 -ArgumentList "--play_file $DEMO_FILE_PATH --config_file_path $CONFIG_FILE_PATH"
$result = $process.ExitCode
	
if ($result -eq 0) {
    Write-Output "Playthough success!"
} else {
    Write-Output "Playthough failed!"
}

Read-Host -Prompt "Press ENTER or CTRL+C to exit" | Out-Null