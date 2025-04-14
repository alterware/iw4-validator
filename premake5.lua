workspace "iw4-validator"
startproject "iw4-validator"
location "./build"
objdir "%{wks.location}/obj"
targetdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

configurations {"debug", "release"}

if os.istarget("linux") then
	platforms {"x86", "amd64", "arm64"}
elseif os.istarget("macosx") then
	platforms {"amd64", "arm64"}
else 
	platforms {"x86", "x64", "arm64"}
end

filter "platforms:x86"
	architecture "x86"
filter {}

filter "platforms:x64"
	architecture "x86_64"
filter {}

filter "platforms:amd64"
 architecture "x86_64"
 filter {}

filter "platforms:arm64"
	architecture "ARM64"
filter {}

filter {"system:windows"}
	systemversion "latest"
filter {}

symbols "On"
staticruntime "On"
editandcontinue "Off"
warnings "Extra"
characterset "ASCII"

filter {"system:linux", "system:macosx"}
	buildoptions "-pthread"
	linkoptions "-pthread"
filter {}

if os.istarget("linux") then
	-- this supports cross-compilation for arm64
	filter { "toolset:clang*", "platforms:arm64" }
		buildoptions "--target=arm64-linux-gnu"
		linkoptions "--target=arm64-linux-gnu"
	filter {}

	filter { "toolset:clang*" }
		-- always try to use lld. LD or Gold will not work
		linkoptions "-fuse-ld=lld"
	filter {}
end

filter { "system:macosx", "platforms:amd64" }
	buildoptions "-arch x86_64"
	linkoptions "-arch x86_64"
filter {}

filter { "system:macosx", "platforms:arm64" }
	buildoptions "-arch arm64"
	linkoptions "-arch arm64"
filter {}

if os.getenv("CI") then
	defines "CI"
end

flags {"NoIncrementalLink", "NoMinimalRebuild", "MultiProcessorCompile", "No64BitChecks"}

filter "configurations:release"
	optimize "Speed"
	defines "NDEBUG"
	fatalwarnings {"All"}
filter {}

filter "configurations:debug"
	optimize "Debug"
	defines {"DEBUG", "_DEBUG"}
filter {}

project "iw4-validator"
kind "ConsoleApp"
language "C++"
cppdialect "C++20"

targetname "iw4-validator"

pchheader "std_include.hpp"
pchsource "src/std_include.cpp"

files {"./src/**.hpp", "./src/**.cpp"}

includedirs {"./src", "%{prj.location}/src"}

filter {"system:windows", "toolset:not msc*"}
	resincludedirs {
		"%{_MAIN_SCRIPT_DIR}/src"
	}
filter {}

filter {"system:windows", "toolset:msc*"}
	resincludedirs {
		"$(ProjectDir)src"
	}
filter {}
