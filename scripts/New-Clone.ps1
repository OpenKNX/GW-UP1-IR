$repo = "GW-UP1-IR"
$oldDir = Get-Location

Write-Output "Klone das Repo $repo" -ForegroundColor Yellow

git clone https://github.com/OpenKnx/$repo

Write-Output "Führe das Skript 'Restore-Project.ps1' aus" -ForegroundColor Yellow

Set-Location $repo\
.\scripts\Restore-Project.ps1 -wait $False

Write-Output "Fertig" -ForegroundColor Yellow
Set-Location $oldDir
timeout /T 20