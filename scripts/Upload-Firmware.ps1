$env = $args[0]

if(-Not (Test-Path -Path platformio.ini)) {
    Set-Location ..
}
if(-Not (Test-Path -Path platformio.ini)) {
    Write-Output "Skript aus falschem Pfad ausgeführt..."
    timeout /T 20
    exit 1
}
$projectDir = Get-Location

if($env -eq $null)
{
    Write-Output "No environment specified. Will search for release..."
    $content = Get-Content $projectDir/platformio.ini -raw
    if($content -match '\[env:(release_.*)\]')
    {
        Write-Output "Found release $($Matches.1)"
        $env = $Matches.1
    } else {
        Write-Output "Didnt found host"
        timeout /T 20
        exit 1
    }
}

if ($IsMacOS -or $IsLinux) { ~/.platformio/penv/bin/pio run -e $env -t upload }
else { ~/.platformio/penv/Scripts/pio.exe run -e $env -t upload } 

timeout /T 20