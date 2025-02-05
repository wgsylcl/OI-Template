while ($true) {
    $ps1FilePath = $MyInvocation.MyCommand.Definition
    $scriptDir = Split-Path $ps1FilePath
    git pull
    git add $scriptDir
    git commit $scriptDir -m "upd"
    git push
    timeout /t 900
}