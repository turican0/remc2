$REMC2_EXECUTABLE = "$PSScriptRoot/../Release/remc2.exe"
$CONFIG_FILE_PATH = "$PSScriptRoot/renderer-config.json"

function Test-Level {
    param (
        [int]$level
    )
    Write-Output "Testing renderer in level $level"
	Write-Output $REMC2_EXECUTABLE
	Write-Output $CONFIG_FILE_PATH
	$process = Start-Process -Wait -PassThru -FilePath $REMC2_EXECUTABLE -ArgumentList "--test_renderers --set_level $level --config_file_path $CONFIG_FILE_PATH"
	$result = $process.ExitCode
	
    if ($result -eq 0) {
        Write-Output "test level $level ok"
    } else {
        Write-Output "test level $level failed"
    }
}

for ($i = 0; $i -le 3; $i++) {
	if($i -ne 2)
	{
		Test-Level -level $i
	}
}

Read-Host -Prompt "Press ENTER or CTRL+C to exit" | Out-Null

$REMC2_EXECUTABLE = "$PSScriptRoot/../x64/Release/remc2.exe"
$CONFIG_FILE_PATH = "$PSScriptRoot/renderer-config.json"

function Test-Level {
    param (
        [int]$level
    )
    Write-Output "Testing renderer in level $level"
	Write-Output $REMC2_EXECUTABLE
	Write-Output $CONFIG_FILE_PATH
	$process = Start-Process -Wait -PassThru -FilePath $REMC2_EXECUTABLE -ArgumentList "--test_renderers --set_level $level --config_file_path $CONFIG_FILE_PATH"
	$result = $process.ExitCode
	
    if ($result -eq 0) {
        Write-Output "test level $level ok"
    } else {
        Write-Output "test level $level failed"
    }
}

for ($i = 0; $i -le 3; $i++) {
	if($i -ne 2)
	{
		Test-Level -level $i
	}
}

Read-Host -Prompt "Press ENTER or CTRL+C to exit" | Out-Null