project(obs)

option(ENABLE_UI "Enables the OBS user interfaces" ON)
if(DISABLE_UI)
	message(STATUS "UI disabled")
	return()
elseif(ENABLE_UI)
	set(FIND_MODE REQUIRED)
else()
	set(FIND_MODE QUIET)
endif()

if(DEFINED QTDIR${_lib_suffix})
	list(APPEND CMAKE_PREFIX_PATH "${QTDIR${_lib_suffix}}")
elseif(DEFINED QTDIR)
	list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")
elseif(DEFINED ENV{QTDIR${_lib_suffix}})
	list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR${_lib_suffix}}")
elseif(DEFINED ENV{QTDIR})
	list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(_lib_suffix 64)
else()
	set(_lib_suffix 32)
endif()

find_package(Qt5Widgets ${FIND_MODE})

if(NOT Qt5Widgets_FOUND)
	if (ENABLE_UI)
		message(FATAL_ERROR "Failed to find Qt5")
	else()
		message(STATUS "Qt5 not found - UI disabled")
		return()
	endif()
endif()
find_package(Qt5WebKitWidgets ${FIND_MODE})

if(NOT Qt5WebKitWidgets_FOUND)
	if (ENABLE_UI)
		message(FATAL_ERROR "Failed to find WebKitWidgets")
	else()
		message(STATUS "Qt5 not found - UI WebKitWidgets")
		return()
	endif()
endif()
find_package(Qt5Network ${FIND_MODE})

if(NOT Qt5Network_FOUND)
	if (ENABLE_UI)
		message(FATAL_ERROR "Failed to find Network")
	else()
		message(STATUS "Qt5 not found - UI disabled")
		return()
	endif()
endif()
find_package(Qt5WebKit ${FIND_MODE})

if(NOT Qt5WebKit_FOUND)
	if (ENABLE_UI)
		message(FATAL_ERROR "Failed to find WebKit")
	else()
		message(STATUS "Qt5 not found - UI disabled")
		return()
	endif()
endif()

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/../OBSlogin")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/../build-OBSlogin-Desktop_Qt_5_4_2_MSVC2013_64bit-Debug")
find_package(Libcurl REQUIRED)
include_directories(${LIBCURL_INCLUDE_DIRS})
add_definitions(${LIBCURL_DEFINITIONS})

if(WIN32)
	set(obs_PLATFORM_SOURCES
		platform-windows.cpp
		obs.rc)
elseif(APPLE)
	set(obs_PLATFORM_SOURCES
		platform-osx.mm)

	find_package(AppKit REQUIRED)
	set(obs_PLATFORM_LIBRARIES ${APPKIT_LIBRARIES})

	add_definitions(-fobjc-arc)

	option(ENABLE_SPARKLE_UPDATER "Enables updates via the Sparkle framework (don't forget to update the Info.plist for your .app)" OFF)
	if(ENABLE_SPARKLE_UPDATER)
		find_library(SPARKLE Sparkle)
		include_directories(${SPARKLE})
		set(obs_PLATFORM_SOURCES
			${obs_PLATFORM_SOURCES}
			sparkle-updater.mm)
		set(obs_PLATFORM_LIBRARIES
			${obs_PLATFORM_LIBRARIES}
			${SPARKLE})
		add_definitions(-DUPDATE_SPARKLE=1)
	endif()

elseif(UNIX)
        find_package(Qt5X11Extras REQUIRED)

	set(obs_PLATFORM_SOURCES
		platform-x11.cpp)

	find_package(XCB COMPONENTS XCB REQUIRED RANDR REQUIRED XINERAMA REQUIRED)
	
	include_directories(
		${XCB_INCLUDE_DIRS}
		${X11_XCB_INCLUDE_DIRS})
		
	add_definitions(
		${XCB_DEFINITIONS}
		${X11_XCB_DEFINITIONS})

	set(obs_PLATFORM_LIBRARIES
		${XCB_LIBRARIES}
		${X11_XCB_LIBRARIES}
                Qt5::X11Extras)
endif()

set(obs_SOURCES
	${obs_PLATFORM_SOURCES}
	obs-app.cpp
	window-basic-main.cpp
	window-basic-filters.cpp
	window-basic-settings.cpp
	window-basic-interaction.cpp
	window-basic-properties.cpp
	window-basic-main-outputs.cpp
	window-basic-source-select.cpp
	window-basic-main-scene-collections.cpp
	window-basic-main-transitions.cpp
	window-basic-main-profiles.cpp
	window-license-agreement.cpp
	window-basic-status-bar.cpp
	window-basic-adv-audio.cpp
	window-basic-transform.cpp
	window-basic-preview.cpp
	window-namedialog.cpp
	window-log-reply.cpp
	window-projector.cpp
	window-remux.cpp
	properties-view.cpp
	focus-list.cpp
	menu-button.cpp
	double-slider.cpp
	volume-control.cpp
	adv-audio-control.cpp
	item-widget-helpers.cpp
	visibility-checkbox.cpp
	vertical-scroll-area.cpp
	visibility-item-widget.cpp
	slider-absoluteset-style.cpp
	source-list-widget.cpp
	qt-display.cpp
	crash-report.cpp
	hotkey-edit.cpp
	source-label.cpp
	remote-text.cpp
	audio-encoders.cpp
	qt-wrappers.cpp
	../../OBSlogin/loginwindow.cpp)

set(obs_HEADERS
	obs-app.hpp
	platform.hpp
	window-main.hpp
	window-basic-main.hpp
	window-basic-filters.hpp
	window-basic-settings.hpp
	window-basic-interaction.hpp
	window-basic-properties.hpp
	window-basic-main-outputs.hpp
	window-basic-source-select.hpp
	window-license-agreement.hpp
	window-basic-status-bar.hpp
	window-basic-adv-audio.hpp
	window-basic-transform.hpp
	window-basic-preview.hpp
	window-namedialog.hpp
	window-log-reply.hpp
	window-projector.hpp
	window-remux.hpp
	properties-view.hpp
	properties-view.moc.hpp
	display-helpers.hpp
	double-slider.hpp
	focus-list.hpp
	menu-button.hpp
	mute-checkbox.hpp
	volume-control.hpp
	adv-audio-control.hpp
	item-widget-helpers.hpp
	visibility-checkbox.hpp
	vertical-scroll-area.hpp
	visibility-item-widget.hpp
	slider-absoluteset-style.hpp
	source-list-widget.hpp
	qt-display.hpp
	crash-report.hpp
	hotkey-edit.hpp
	source-label.hpp
	remote-text.hpp
	audio-encoders.hpp
	qt-wrappers.hpp
	../../OBSlogin/loginwindow.h
	../../build-OBSlogin-Desktop_Qt_5_4_2_MSVC2013_64bit-Debug/ui_loginwindow.h)

set(obs_UI
	forms/NameDialog.ui
	forms/OBSLicenseAgreement.ui
	forms/OBSLogReply.ui
	forms/OBSBasic.ui
	forms/OBSBasicTransform.ui
	forms/OBSBasicFilters.ui
	forms/OBSBasicSettings.ui
	forms/OBSBasicSourceSelect.ui
	forms/OBSBasicInteraction.ui
	forms/OBSRemux.ui)

set(obs_QRC
	forms/obs.qrc)

qt5_wrap_ui(obs_UI_HEADERS ${obs_UI})
qt5_add_resources(obs_QRC_SOURCES ${obs_QRC})

add_executable(obs WIN32
	${obs_SOURCES}
	${obs_HEADERS}
	${obs_UI_HEADERS}
	${obs_QRC_SOURCES})

if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_output_suffix "64")
	else()
		set(_output_suffix "32")
	endif()

	set_target_properties(obs
		PROPERTIES
			OUTPUT_NAME "obs${_output_suffix}")
endif()

target_link_libraries(obs
	libobs
	libff
	Qt5::Widgets
	Qt5::WebKit
	Qt5::WebKitWidgets
	Qt5::Network
	${LIBCURL_LIBRARIES}
	${obs_PLATFORM_LIBRARIES})
	#doan nay moi
	if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		add_custom_command(TARGET obs POST_BUILD 
		COMMAND if $(ConfigurationName) == Debug (
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebKitWidgetsd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"       \n          # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebKitd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"         \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Sensorsd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"          \n        # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Positioningd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"           \n        # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Quickd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"             \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Qmld.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"             \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Networkd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"              \n     # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Multimediad.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"               \n    # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebChanneld.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"               \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Sqld.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5MultimediaWidgetsd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5OpenGLd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5PrintSupportd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"                \n   # <--this is out-file path
		xcopy /S /y "$(DepsPath)\\..\\dll" "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit" \n
		xcopy /S /y "$(QTDIR)\\plugins\\imageformats" "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit\\plugins\\imageformats\\" \n
		)
		else
		(
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebKitWidgets.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"       \n          # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebKit.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"         \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Sensors.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"          \n        # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Positioning.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"           \n        # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Quick.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"             \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Qml.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"             \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Network.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"              \n     # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Multimedia.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"               \n    # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebChannel.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"               \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Sql.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5MultimediaWidgets.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5OpenGL.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5PrintSupport.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit"                \n   # <--this is out-file path
		xcopy /S /y "$(DepsPath)\\..\\dll" "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit" \n
		xcopy /S /y "$(QTDIR)\\plugins\\imageformats" "$(SolutionDir)rundir\\$(Configuration)\\bin\\64bit\\plugins\\imageformats\\" \n
		)		# <--this is out-file path
		)
	else()
		add_custom_command(TARGET obs POST_BUILD 
		COMMAND if $(ConfigurationName) == Debug (
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebKitWidgetsd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"       \n          # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebKitd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"         \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Sensorsd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"           \n       # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Positioningd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"             \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Quickd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"              \n     # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Qmld.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"               \n    # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Networkd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"              \n     # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Multimediad.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"               \n    # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebChanneld.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"               \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Sqld.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5MultimediaWidgetsd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5OpenGLd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"                 \n  # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5PrintSupportd.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"               \n    # <--this is out-file path
		xcopy /S /y "$(DepsPath)\\..\\dll" "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit" \n
		xcopy /S /y "$(QTDIR)\\plugins\\imageformats" "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit\\plugins\\imageformats\\" \n
				) 
			else(
			xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebKitWidgets.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"       \n          # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebKit.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"         \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Sensors.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"           \n       # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Positioning.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"             \n      # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Quick.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"              \n     # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Qml.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"               \n    # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Network.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"              \n     # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Multimedia.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"               \n    # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5WebChannel.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"               \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5Sql.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5MultimediaWidgets.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"                \n   # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5OpenGL.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"                 \n  # <--this is out-file path
		xcopy /y /d  # which executes "cmake - E copy_if_different..."
        "$(QTDIR)\\bin\\Qt5PrintSupport.dll"      # <--this is in-file
        "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit"               \n    # <--this is out-file path
		xcopy /S /y "$(DepsPath)\\..\\dll" "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit" \n
		xcopy /S /y "$(QTDIR)\\plugins\\imageformats" "$(SolutionDir)rundir\\$(Configuration)\\bin\\32bit\\plugins\\imageformats\\" \n
			)				
		)		# <--this is out-file path
	endif()

	endif()

	#het doan moi
define_graphic_modules(obs)

install_obs_core(obs)
install_obs_data(obs data obs-studio)

if (UNIX AND UNIX_STRUCTURE AND NOT APPLE)
	install(FILES dist/obs.desktop DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/applications)
	install(FILES forms/images/obs.png
		DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/icons/hicolor/256x256/apps)
endif()
