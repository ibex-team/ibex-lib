$ErrorActionPreference = 'Stop'; # Stop on all errors.

# Source registry key values which are shared between install and uninstall.
. $PSScriptRoot\regKeys.ps1

#Uninstall-BinFile -Name libibex.a
#Uninstall-BinFile -Name libprim.a

if (Test-Path $CMakeRegistryPath) {
  if (Test-Path $CMakeSystemRepositoryPath) {
      Remove-Item "$CMakeSystemRepositoryPath\$CMakePackageName"
  }
}
