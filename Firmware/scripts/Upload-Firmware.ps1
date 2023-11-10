$env = $args[0]

if(!Test-Path -Path $projectDir/.pio/platformio.base.ini) {
    Write-Host "Lade benötigte inis..."
    scripts/Restore-First.ps1
}

if ($IsMacOS -or $IsLinux) { ~/.platformio/penv/bin/pio run -e $env }
else { ~/.platformio/penv/Scripts/pio.exe run -e $env } 