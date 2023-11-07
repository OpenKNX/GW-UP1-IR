# we assume, we start this script in projects "restore" directory
$currentDir = Get-Location
Set-Location ..
$subprojects_dependencies = Get-Content platformio.ini
$projectDir = Get-Location

if(Test-Path -Path $projectDir/.pio/platformio.base.ini) {
    Write-Host "Lösche 'platformio.rp2040.ini'"
    Remove-Item $projectDir/.pio/platformio.base.ini
}

if(Test-Path -Path $projectDir/.pio/platformio.rp2040.ini) {
    Write-Host "Lösche 'platformio.rp2040.ini'"
    Remove-Item $projectDir/.pio/platformio.rp2040.ini
}

Write-Host "Lade 'platformio.base.ini' herunter..."
Invoke-WebRequest https://raw.githubusercontent.com/OpenKNX/OGM-Common/v1/platformio.base.ini -OutFile $projectDir/.pio/platformio.base.ini
Write-Host "Lade 'platformio.rp2040.ini' herunter..."
Invoke-WebRequest https://raw.githubusercontent.com/OpenKNX/OGM-Common/v1/platformio.rp2040.ini -OutFile $projectDir/.pio/platformio.rp2040.ini
Write-Host "Fertig!"

timeout /T 20