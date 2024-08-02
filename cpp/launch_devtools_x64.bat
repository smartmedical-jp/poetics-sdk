pushd %~dp0
set script_dir=%CD%
popd
%comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cd %script_dir%