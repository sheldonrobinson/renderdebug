@echo off
cd bin
cd win32

copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win32\RenderDebug_x86.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win32\DebugView.exe
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win32\ApexFramework_x86.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win32\PhysX3_x86.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win32\PhysX3Gpu_x86.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win32\PhysX3Common_x86.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win32\PhysX3Cooking_x86.dll

cd ..
cd win64
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win64\RenderDebug_x64.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win64\DebugView.exe
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win64\ApexFramework_x64.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win64\PhysX3_x64.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win64\PhysX3Gpu_x64.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win64\PhysX3Common_x64.dll
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\bin\win64\PhysX3Cooking_x64.dll
cd ..
cd ..
cd public
cd RenderDebug
copy d:\p4\sw\physx\PET\1.3.2\feature\DST\public\RenderDebug\*.*
cd ..
cd ..
