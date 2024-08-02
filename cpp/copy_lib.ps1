param(
    [string]$src_dir,
    [string]$dst_root,
    [string]$build_type
)

function Get-MsvcArch {
    $resultPath = ""
    $env:Path -split ';' | ForEach-Object {
        $path = $_
        if (Test-Path "$path\cl.exe") {
            $resultPath = $path
            return
        }
    }

    if ($resultPath) {
        # Write-Host "Found cl.exe at $resultPath"
        # Get last part of path, e.g. "x64" only from $resultPath
        $arch = $resultPath -split '\\' | Select-Object -Last 1
        return $arch
    }
    else {
        Write-Host "cl.exe not found"
        return $null
    }
}

$arch = Get-MsvcArch
Write-Host "Source directory: $src_dir"
$dst_dir = "$dst_root/$arch/$build_type"
Write-Host "Destination directory: $dst_dir"

if (-not (Test-Path -Path $dst_dir)) {
    [void](New-Item -Path $dst_dir -ItemType Directory)
}
else {
    Remove-Item -Path $dst_dir -Recurse -Force
}

# Get list of every file in $src_dir
Get-ChildItem -Path $src_dir -Recurse | ForEach-Object {
    Copy-Item -Path $_.FullName -Destination $dst_dir
}

Write-Host "Copied artifacts"