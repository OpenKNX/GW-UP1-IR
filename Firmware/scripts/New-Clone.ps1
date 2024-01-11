$repo = "GW-UP1-IR"
$oldDir = Get-Location

Write-Host "Klone das Repo $repo" -ForegroundColor Yellow

git clone https://github.com/OpenKnx/$repo

Write-Host "Führe das Skript 'Restore-Project.ps1' aus" -ForegroundColor Yellow

Set-Location $repo\Firmware\
.\scripts\Restore-Project.ps1 -wait $False

Write-Host "Fertig" -ForegroundColor Yellow
Set-Location $oldDir
timeout /T 20