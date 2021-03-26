$ErrorActionPreference = 'Stop'; # stop on all errors

# Source registry key values which are shared between install and uninstall
. $PSScriptRoot\regKeys.ps1

# CMake doesn't set registry keys for the system package repository by default so we check for the presence of CMake by observing the registry path for the current user.
# We need to register the package path to the machine because the jenkins node user doesn't have privileges to install packages via chocolatey. There may be other options to work around this and use HKCU.
if (Test-Path $CMakeRegistryPath) {
  if (-not(Test-Path $CMakeSystemRepositoryPath)) {
    # The force is necessary to make this create parent keys (think `mkdir -p`). It ain't great.
    New-Item $CMakeSystemRepositoryPath -ItemType directory -Force
  }

  New-Item "$CMakeSystemRepositoryPath\$CMakePackageName" -ItemType directory -Force
  New-ItemProperty -Name "CMakePackageDir" -PropertyType String -Value "$env:ChocolateyPackageFolder\share\$CMakePackageName\cmake" -Path "$CMakeSystemRepositoryPath\$CMakePackageName" -Force
}