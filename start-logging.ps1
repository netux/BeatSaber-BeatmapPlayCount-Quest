Param(
    [Parameter(Mandatory=$false)]
    [Switch] $self,

    [Parameter(Mandatory=$false)]
    [Switch] $all,

    [Parameter(Mandatory=$false)]
    [String] $custom="",

    [Parameter(Mandatory=$false)]
    [String] $file="",

    [Parameter(Mandatory=$false)]
    [Switch] $help,

    [Parameter(Mandatory=$false)]
    [Switch] $excludeHeader
)

if ($help -eq $true) {
    if ($excludeHeader -eq $false) {
        Write-Output "`"Start-Logging`" - Logs Beat Saber using `"adb logcat`""
        Write-Output "`n-- Arguments --`n"
    }

    Write-Output "-Self `t`t Only Logs your mod and Crashes"
    Write-Output "-All `t`t Logs everything, including logs made by the Quest itself"
    Write-Output "-Custom `t Specify a specific logging pattern, e.g `"custom-types|questui`""
    Write-Output "`t`t NOTE: The paterent `"AndriodRuntime|CRASH`" is always appended to a custom pattern"
    Write-Output "-File `t`t Saves the output of the log to the file name given"

    exit
}

function Get-Adb-LogCat-Command ($fileAppend) {
    $command = "adb logcat "

    if ($all -eq $false) {
        Write-Host "Getting Beat Saber process ID... " -NoNewline

        $loops = 0
        while ([string]::IsNullOrEmpty($bspid) -and $loops -lt 5) {
            Start-Sleep -Milliseconds 1000
            $bspid = adb shell pidof com.beatgames.beatsaber
            $loops += 1
        }

        if ([string]::IsNullOrEmpty($bspid)) {
            Write-Host ""
            Write-Host "Could not connect to adb, exiting..."
            exit 1
        } else {
            Write-Host $bspid
        }

        $command += "--pid $bspid"
    }

    if ($all -eq $false) {
        $pattern = "("
        if ($self -eq $true) {
            $modID = (Get-Content "./mod.json" -Raw | ConvertFrom-Json).id
            $pattern += "$modID|"
        }
        if (![string]::IsNullOrEmpty($custom)) {
            $pattern += "$custom|"
        }
        if ($pattern -eq "(") {
            $pattern = "(QuestHook|modloader|"
        }
        $pattern += "AndroidRuntime|CRASH)"
        $command += " | Select-String -pattern `"$pattern`""
    }

    if (![string]::IsNullOrEmpty($file)) {
        $command += " | Tee-Object "
        if ($fileAppend -eq $true) {
            $command += "-Append "
        }
        $command += "-FilePath $PSScriptRoot\$file"
    }

    return $command
}

$firstTry = $true;
do {
    try {
        $command = Get-Adb-LogCat-Command (-not $firstTry)

        if (-not $firstTry) {
            Write-Host "Logging using Command `"$command`""
        }
        $firstTry = $false
        Invoke-Expression $command
    } catch {
        Write-Error $_
        break
    }
} while ($true)
