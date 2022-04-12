$ErrorActionPreference = 'Stop'; # Stop on all errors.

# Source registry key values which are shared between install and uninstall.
. $PSScriptRoot\regKeys.ps1

New-Item "$CMakeSystemRepositoryPath\$CMakePackageName" -ItemType directory -Force
New-ItemProperty -Name "CMakePackageDir" -PropertyType String -Value "$env:ChocolateyPackageFolder\share\$CMakePackageName\cmake" -Path "$CMakeSystemRepositoryPath\$CMakePackageName" -Force

# Get 32 bit binaries if needed and delete unnecessary files...
if (Test-Path "$env:ChocolateyPackageFolder\x86") {
	if ((Get-ProcessorBits 32) -or $env:ChocolateyForceX86 -eq $true) {
		Copy-Item -Recurse -Force -Path "$env:ChocolateyPackageFolder\x86\*" -Destination "$env:ChocolateyPackageFolder"
	}
	Remove-Item -Recurse -Force "$env:ChocolateyPackageFolder\x86"
}

#Install-BinFile -Name libprim.a -Path "$env:ChocolateyPackageFolder\lib\ibex\3rd"
#Install-BinFile -Name libibex.a -Path "$env:ChocolateyPackageFolder\lib"
