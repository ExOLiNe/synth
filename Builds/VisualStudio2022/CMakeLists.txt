cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(Synth C CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################



























set(PROJECT_NAME Synth_StandalonePlugin)

################################################################################
# Source groups
################################################################################
set(JUCE_Library_Code
    "../../JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp"
)
source_group("JUCE Library Code" FILES ${JUCE_Library_Code})

set(JUCE_Modules__juce_audio_plugin_client
#    "../../../JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_Standalone.cpp"
)
source_group("JUCE Modules\\juce_audio_plugin_client" FILES ${JUCE_Modules__juce_audio_plugin_client})

set(JUCE_Modules__juce_audio_plugin_client__Standalone
#    "../../../JUCE/modules/juce_audio_plugin_client/Standalone/juce_StandaloneFilterApp.cpp"
    "../../../JUCE/modules/juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h"
)
source_group("JUCE Modules\\juce_audio_plugin_client\\Standalone" FILES ${JUCE_Modules__juce_audio_plugin_client__Standalone})

set(ALL_FILES
    ${JUCE_Library_Code}
    ${JUCE_Modules__juce_audio_plugin_client}
    ${JUCE_Modules__juce_audio_plugin_client__Standalone}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "Synth"
    TARGET_NAME_RELEASE "Synth"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/Standalone Plugin"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/Standalone Plugin"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    PDB_OUTPUT_DIRECTORY         "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_DEBUG   "FALSE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
    >
    $<$<CONFIG:Release>:
        MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../maximilian"
    ">"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../JuceLibraryCode;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../JUCE/modules"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "DEBUG;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "_CRT_SECURE_NO_WARNINGS;"
    "WIN32;"
    "_WINDOWS;"
    "JUCE_DISPLAY_SPLASH_SCREEN=1;"
    "JUCE_USE_DARK_SPLASH_SCREEN=1;"
    "JUCE_PROJUCER_VERSION=0x70003;"
    "JUCE_MODULE_AVAILABLE_juce_audio_basics=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_devices=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_formats=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_plugin_client=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_processors=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_utils=1;"
    "JUCE_MODULE_AVAILABLE_juce_core=1;"
    "JUCE_MODULE_AVAILABLE_juce_data_structures=1;"
    "JUCE_MODULE_AVAILABLE_juce_dsp=1;"
    "JUCE_MODULE_AVAILABLE_juce_events=1;"
    "JUCE_MODULE_AVAILABLE_juce_graphics=1;"
    "JUCE_MODULE_AVAILABLE_juce_gui_basics=1;"
    "JUCE_MODULE_AVAILABLE_juce_gui_extra=1;"
    "JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1;"
    "JUCE_VST3_CAN_REPLACE_VST2=0;"
    "JUCE_STRICT_REFCOUNTEDPOINTER=1;"
    "JucePlugin_Build_VST=0;"
    "JucePlugin_Build_VST3=0;"
    "JucePlugin_Build_AU=0;"
    "JucePlugin_Build_AUv3=0;"
    "JucePlugin_Build_AAX=0;"
    "JucePlugin_Build_Standalone=1;"
    "JucePlugin_Build_Unity=0;"
    "JucePlugin_Build_LV2=0;"
    "JucePlugin_Enable_IAA=0;"
    "JucePlugin_Enable_ARA=0;"
    "JucePlugin_Name=\"Synth\";"
    "JucePlugin_Desc=\"Synth\";"
    "JucePlugin_Manufacturer=\"yourcompany\";"
    "JucePlugin_ManufacturerWebsite=\"www.yourcompany.com\";"
    "JucePlugin_ManufacturerEmail=\"\";"
    "JucePlugin_ManufacturerCode=0x4d616e75;"
    "JucePlugin_PluginCode=0x43626b37;"
    "JucePlugin_IsSynth=1;"
    "JucePlugin_WantsMidiInput=1;"
    "JucePlugin_ProducesMidiOutput=1;"
    "JucePlugin_IsMidiEffect=0;"
    "JucePlugin_EditorRequiresKeyboardFocus=0;"
    "JucePlugin_Version=1.0.0;"
    "JucePlugin_VersionCode=0x10000;"
    "JucePlugin_VersionString=\"1.0.0\";"
    "JucePlugin_VSTUniqueID=JucePlugin_PluginCode;"
    "JucePlugin_VSTCategory=kPlugCategSynth;"
    "JucePlugin_Vst3Category=\"Instrument\";"
    "JucePlugin_AUMainType='aumu';"
    "JucePlugin_AUSubType=JucePlugin_PluginCode;"
    "JucePlugin_AUExportPrefix=SynthAU;"
    "JucePlugin_AUExportPrefixQuoted=\"SynthAU\";"
    "JucePlugin_AUManufacturerCode=JucePlugin_ManufacturerCode;"
    "JucePlugin_CFBundleIdentifier=com.yourcompany.Synth;"
    "JucePlugin_AAXIdentifier=com.yourcompany.Synth;"
    "JucePlugin_AAXManufacturerCode=JucePlugin_ManufacturerCode;"
    "JucePlugin_AAXProductId=JucePlugin_PluginCode;"
    "JucePlugin_AAXCategory=2048;"
    "JucePlugin_AAXDisableBypass=0;"
    "JucePlugin_AAXDisableMultiMono=0;"
    "JucePlugin_IAAType=0x61757269;"
    "JucePlugin_IAASubType=JucePlugin_PluginCode;"
    "JucePlugin_IAAName=\"yourcompany: Synth\";"
    "JucePlugin_VSTNumMidiInputs=16;"
    "JucePlugin_VSTNumMidiOutputs=16;"
    "JucePlugin_ARAContentTypes=0;"
    "JucePlugin_ARATransformationFlags=0;"
    "JucePlugin_ARAFactoryID=\"com.yourcompany.Synth.factory\";"
    "JucePlugin_ARADocumentArchiveID=\"com.yourcompany.Synth.aradocumentarchive.1.0.0\";"
    "JucePlugin_ARACompatibleArchiveIDs=\"\";"
    "JUCE_STANDALONE_APPLICATION=JucePlugin_Build_Standalone;"
    "JUCER_VS2022_78A503E=1;"
    "JUCE_APP_VERSION=1.0.0;"
    "JUCE_APP_VERSION_HEX=0x10000"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Zi
        >
        $<$<CONFIG:Release>:
            /Ox;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /MP;
        /std:c++17;
        /W4;
        /GR;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /NODEFAULTLIB:libcmt.lib;
            /NODEFAULTLIB:msvcrt.lib;
            /DEBUG
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /LTCG;
            /OPT:ICF
        >
        /NOLOGO;
        /MANIFEST;
        /SUBSYSTEM:WINDOWS
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Synth - Shared Code
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "Synth"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")



























set(PROJECT_NAME Synth_VST3)

################################################################################
# Source groups
################################################################################
set(JUCE_Library_Code
    "../../JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp"
)
source_group("JUCE Library Code" FILES ${JUCE_Library_Code})

set(JUCE_Modules__juce_audio_plugin_client
#    "../../../JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_VST3.cpp"
)
source_group("JUCE Modules\\juce_audio_plugin_client" FILES ${JUCE_Modules__juce_audio_plugin_client})

set(JUCE_Modules__juce_audio_plugin_client__VST3
#    "../../../JUCE/modules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp"
)
source_group("JUCE Modules\\juce_audio_plugin_client\\VST3" FILES ${JUCE_Modules__juce_audio_plugin_client__VST3})

set(ALL_FILES
    ${JUCE_Library_Code}
    ${JUCE_Modules__juce_audio_plugin_client}
    ${JUCE_Modules__juce_audio_plugin_client__VST3}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "Synth"
    TARGET_NAME_RELEASE "Synth"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/VST3"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/VST3"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    PDB_OUTPUT_DIRECTORY         "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_DEBUG   "FALSE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
    >
    $<$<CONFIG:Release>:
        MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../maximilian"
    ">"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../JuceLibraryCode;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../JUCE/modules"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "DEBUG;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "_CRT_SECURE_NO_WARNINGS;"
    "WIN32;"
    "_WINDOWS;"
    "JUCE_DISPLAY_SPLASH_SCREEN=1;"
    "JUCE_USE_DARK_SPLASH_SCREEN=1;"
    "JUCE_PROJUCER_VERSION=0x70003;"
    "JUCE_MODULE_AVAILABLE_juce_audio_basics=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_devices=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_formats=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_plugin_client=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_processors=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_utils=1;"
    "JUCE_MODULE_AVAILABLE_juce_core=1;"
    "JUCE_MODULE_AVAILABLE_juce_data_structures=1;"
    "JUCE_MODULE_AVAILABLE_juce_dsp=1;"
    "JUCE_MODULE_AVAILABLE_juce_events=1;"
    "JUCE_MODULE_AVAILABLE_juce_graphics=1;"
    "JUCE_MODULE_AVAILABLE_juce_gui_basics=1;"
    "JUCE_MODULE_AVAILABLE_juce_gui_extra=1;"
    "JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1;"
    "JUCE_VST3_CAN_REPLACE_VST2=0;"
    "JUCE_STRICT_REFCOUNTEDPOINTER=1;"
    "JucePlugin_Build_VST=0;"
    "JucePlugin_Build_VST3=1;"
    "JucePlugin_Build_AU=0;"
    "JucePlugin_Build_AUv3=0;"
    "JucePlugin_Build_AAX=0;"
    "JucePlugin_Build_Standalone=0;"
    "JucePlugin_Build_Unity=0;"
    "JucePlugin_Build_LV2=0;"
    "JucePlugin_Enable_IAA=0;"
    "JucePlugin_Enable_ARA=0;"
    "JucePlugin_Name=\"Synth\";"
    "JucePlugin_Desc=\"Synth\";"
    "JucePlugin_Manufacturer=\"yourcompany\";"
    "JucePlugin_ManufacturerWebsite=\"www.yourcompany.com\";"
    "JucePlugin_ManufacturerEmail=\"\";"
    "JucePlugin_ManufacturerCode=0x4d616e75;"
    "JucePlugin_PluginCode=0x43626b37;"
    "JucePlugin_IsSynth=1;"
    "JucePlugin_WantsMidiInput=1;"
    "JucePlugin_ProducesMidiOutput=1;"
    "JucePlugin_IsMidiEffect=0;"
    "JucePlugin_EditorRequiresKeyboardFocus=0;"
    "JucePlugin_Version=1.0.0;"
    "JucePlugin_VersionCode=0x10000;"
    "JucePlugin_VersionString=\"1.0.0\";"
    "JucePlugin_VSTUniqueID=JucePlugin_PluginCode;"
    "JucePlugin_VSTCategory=kPlugCategSynth;"
    "JucePlugin_Vst3Category=\"Instrument\";"
    "JucePlugin_AUMainType='aumu';"
    "JucePlugin_AUSubType=JucePlugin_PluginCode;"
    "JucePlugin_AUExportPrefix=SynthAU;"
    "JucePlugin_AUExportPrefixQuoted=\"SynthAU\";"
    "JucePlugin_AUManufacturerCode=JucePlugin_ManufacturerCode;"
    "JucePlugin_CFBundleIdentifier=com.yourcompany.Synth;"
    "JucePlugin_AAXIdentifier=com.yourcompany.Synth;"
    "JucePlugin_AAXManufacturerCode=JucePlugin_ManufacturerCode;"
    "JucePlugin_AAXProductId=JucePlugin_PluginCode;"
    "JucePlugin_AAXCategory=2048;"
    "JucePlugin_AAXDisableBypass=0;"
    "JucePlugin_AAXDisableMultiMono=0;"
    "JucePlugin_IAAType=0x61757269;"
    "JucePlugin_IAASubType=JucePlugin_PluginCode;"
    "JucePlugin_IAAName=\"yourcompany: Synth\";"
    "JucePlugin_VSTNumMidiInputs=16;"
    "JucePlugin_VSTNumMidiOutputs=16;"
    "JucePlugin_ARAContentTypes=0;"
    "JucePlugin_ARATransformationFlags=0;"
    "JucePlugin_ARAFactoryID=\"com.yourcompany.Synth.factory\";"
    "JucePlugin_ARADocumentArchiveID=\"com.yourcompany.Synth.aradocumentarchive.1.0.0\";"
    "JucePlugin_ARACompatibleArchiveIDs=\"\";"
    "JUCE_STANDALONE_APPLICATION=JucePlugin_Build_Standalone;"
    "JUCER_VS2022_78A503E=1;"
    "JUCE_APP_VERSION=1.0.0;"
    "JUCE_APP_VERSION_HEX=0x10000"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Zi
        >
        $<$<CONFIG:Release>:
            /Ox;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /MP;
        /std:c++17;
        /W4;
        /GR;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /NODEFAULTLIB:libcmt.lib;
            /NODEFAULTLIB:msvcrt.lib;
            /DEBUG
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /LTCG;
            /OPT:ICF
        >
        /NOLOGO;
        /MANIFEST;
        /SUBSYSTEM:WINDOWS
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Synth - Shared Code
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "Synth"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")



























set(PROJECT_NAME Synth_SharedCode)

################################################################################
# Source groups
################################################################################
set(JUCE_Library_Code
    "../../JuceLibraryCode/BinaryData.cpp"
    "../../JuceLibraryCode/BinaryData.h"
    "../../JuceLibraryCode/include_juce_audio_basics.cpp"
    "../../JuceLibraryCode/include_juce_audio_devices.cpp"
    "../../JuceLibraryCode/include_juce_audio_formats.cpp"
    "../../JuceLibraryCode/include_juce_audio_plugin_client_ARA.cpp"
    "../../JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "../../JuceLibraryCode/include_juce_audio_processors.cpp"
    "../../JuceLibraryCode/include_juce_audio_processors_ara.cpp"
    "../../JuceLibraryCode/include_juce_audio_processors_lv2_libs.cpp"
    "../../JuceLibraryCode/include_juce_audio_utils.cpp"
    "../../JuceLibraryCode/include_juce_core.cpp"
    "../../JuceLibraryCode/include_juce_data_structures.cpp"
    "../../JuceLibraryCode/include_juce_dsp.cpp"
    "../../JuceLibraryCode/include_juce_events.cpp"
    "../../JuceLibraryCode/include_juce_graphics.cpp"
    "../../JuceLibraryCode/include_juce_gui_basics.cpp"
    "../../JuceLibraryCode/include_juce_gui_extra.cpp"
    "../../JuceLibraryCode/JuceHeader.h"
    "../../JuceLibraryCode/JucePluginDefines.h"
)
source_group("JUCE Library Code" FILES ${JUCE_Library_Code})

set(JUCE_Modules__juce_audio_basics
#    "../../../JUCE/modules/juce_audio_basics/juce_audio_basics.cpp"
    "../../../JUCE/modules/juce_audio_basics/juce_audio_basics.h"
)
source_group("JUCE Modules\\juce_audio_basics" FILES ${JUCE_Modules__juce_audio_basics})

set(JUCE_Modules__juce_audio_basics__audio_play_head
#    "../../../JUCE/modules/juce_audio_basics/audio_play_head/juce_AudioPlayHead.cpp"
    "../../../JUCE/modules/juce_audio_basics/audio_play_head/juce_AudioPlayHead.h"
)
source_group("JUCE Modules\\juce_audio_basics\\audio_play_head" FILES ${JUCE_Modules__juce_audio_basics__audio_play_head})

set(JUCE_Modules__juce_audio_basics__buffers
#    "../../../JUCE/modules/juce_audio_basics/buffers/juce_AudioChannelSet.cpp"
    "../../../JUCE/modules/juce_audio_basics/buffers/juce_AudioChannelSet.h"
#    "../../../JUCE/modules/juce_audio_basics/buffers/juce_AudioDataConverters.cpp"
    "../../../JUCE/modules/juce_audio_basics/buffers/juce_AudioDataConverters.h"
#    "../../../JUCE/modules/juce_audio_basics/buffers/juce_AudioProcessLoadMeasurer.cpp"
    "../../../JUCE/modules/juce_audio_basics/buffers/juce_AudioProcessLoadMeasurer.h"
    "../../../JUCE/modules/juce_audio_basics/buffers/juce_AudioSampleBuffer.h"
#    "../../../JUCE/modules/juce_audio_basics/buffers/juce_FloatVectorOperations.cpp"
    "../../../JUCE/modules/juce_audio_basics/buffers/juce_FloatVectorOperations.h"
)
source_group("JUCE Modules\\juce_audio_basics\\buffers" FILES ${JUCE_Modules__juce_audio_basics__buffers})

set(JUCE_Modules__juce_audio_basics__midi
#    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiBuffer.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiBuffer.h"
    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiDataConcatenator.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiFile.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiFile.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiKeyboardState.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiKeyboardState.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiMessage.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiMessage.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiMessageSequence.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiMessageSequence.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiRPN.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/juce_MidiRPN.h"
)
source_group("JUCE Modules\\juce_audio_basics\\midi" FILES ${JUCE_Modules__juce_audio_basics__midi})

set(JUCE_Modules__juce_audio_basics__midi__ump
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMP.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMP_test.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPacket.h"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPackets.h"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPConversion.h"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPConverters.h"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPDispatcher.h"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPFactory.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPIterator.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPIterator.h"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPMidi1ToBytestreamTranslator.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPMidi1ToMidi2DefaultTranslator.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPMidi1ToMidi2DefaultTranslator.h"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPProtocols.h"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPReceiver.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPSysEx7.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPSysEx7.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPUtils.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPUtils.h"
#    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPView.cpp"
    "../../../JUCE/modules/juce_audio_basics/midi/ump/juce_UMPView.h"
)
source_group("JUCE Modules\\juce_audio_basics\\midi\\ump" FILES ${JUCE_Modules__juce_audio_basics__midi__ump})

set(JUCE_Modules__juce_audio_basics__mpe
#    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEInstrument.cpp"
    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEInstrument.h"
#    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEMessages.cpp"
    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEMessages.h"
#    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPENote.cpp"
    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPENote.h"
#    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPESynthesiser.cpp"
    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPESynthesiser.h"
#    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPESynthesiserBase.cpp"
    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPESynthesiserBase.h"
#    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPESynthesiserVoice.cpp"
    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPESynthesiserVoice.h"
#    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEUtils.cpp"
    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEUtils.h"
#    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEValue.cpp"
    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEValue.h"
#    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEZoneLayout.cpp"
    "../../../JUCE/modules/juce_audio_basics/mpe/juce_MPEZoneLayout.h"
)
source_group("JUCE Modules\\juce_audio_basics\\mpe" FILES ${JUCE_Modules__juce_audio_basics__mpe})

set(JUCE_Modules__juce_audio_basics__native
    "../../../JUCE/modules/juce_audio_basics/native/juce_mac_CoreAudioLayouts.h"
    "../../../JUCE/modules/juce_audio_basics/native/juce_mac_CoreAudioTimeConversions.h"
)
source_group("JUCE Modules\\juce_audio_basics\\native" FILES ${JUCE_Modules__juce_audio_basics__native})

set(JUCE_Modules__juce_audio_basics__sources
    "../../../JUCE/modules/juce_audio_basics/sources/juce_AudioSource.h"
#    "../../../JUCE/modules/juce_audio_basics/sources/juce_BufferingAudioSource.cpp"
    "../../../JUCE/modules/juce_audio_basics/sources/juce_BufferingAudioSource.h"
#    "../../../JUCE/modules/juce_audio_basics/sources/juce_ChannelRemappingAudioSource.cpp"
    "../../../JUCE/modules/juce_audio_basics/sources/juce_ChannelRemappingAudioSource.h"
#    "../../../JUCE/modules/juce_audio_basics/sources/juce_IIRFilterAudioSource.cpp"
    "../../../JUCE/modules/juce_audio_basics/sources/juce_IIRFilterAudioSource.h"
#    "../../../JUCE/modules/juce_audio_basics/sources/juce_MemoryAudioSource.cpp"
    "../../../JUCE/modules/juce_audio_basics/sources/juce_MemoryAudioSource.h"
#    "../../../JUCE/modules/juce_audio_basics/sources/juce_MixerAudioSource.cpp"
    "../../../JUCE/modules/juce_audio_basics/sources/juce_MixerAudioSource.h"
#    "../../../JUCE/modules/juce_audio_basics/sources/juce_PositionableAudioSource.cpp"
    "../../../JUCE/modules/juce_audio_basics/sources/juce_PositionableAudioSource.h"
#    "../../../JUCE/modules/juce_audio_basics/sources/juce_ResamplingAudioSource.cpp"
    "../../../JUCE/modules/juce_audio_basics/sources/juce_ResamplingAudioSource.h"
#    "../../../JUCE/modules/juce_audio_basics/sources/juce_ReverbAudioSource.cpp"
    "../../../JUCE/modules/juce_audio_basics/sources/juce_ReverbAudioSource.h"
#    "../../../JUCE/modules/juce_audio_basics/sources/juce_ToneGeneratorAudioSource.cpp"
    "../../../JUCE/modules/juce_audio_basics/sources/juce_ToneGeneratorAudioSource.h"
)
source_group("JUCE Modules\\juce_audio_basics\\sources" FILES ${JUCE_Modules__juce_audio_basics__sources})

set(JUCE_Modules__juce_audio_basics__synthesisers
#    "../../../JUCE/modules/juce_audio_basics/synthesisers/juce_Synthesiser.cpp"
    "../../../JUCE/modules/juce_audio_basics/synthesisers/juce_Synthesiser.h"
)
source_group("JUCE Modules\\juce_audio_basics\\synthesisers" FILES ${JUCE_Modules__juce_audio_basics__synthesisers})

set(JUCE_Modules__juce_audio_basics__utilities
    "../../../JUCE/modules/juce_audio_basics/utilities/juce_ADSR.h"
#    "../../../JUCE/modules/juce_audio_basics/utilities/juce_ADSR_test.cpp"
    "../../../JUCE/modules/juce_audio_basics/utilities/juce_Decibels.h"
    "../../../JUCE/modules/juce_audio_basics/utilities/juce_GenericInterpolator.h"
#    "../../../JUCE/modules/juce_audio_basics/utilities/juce_IIRFilter.cpp"
    "../../../JUCE/modules/juce_audio_basics/utilities/juce_IIRFilter.h"
#    "../../../JUCE/modules/juce_audio_basics/utilities/juce_Interpolators.cpp"
    "../../../JUCE/modules/juce_audio_basics/utilities/juce_Interpolators.h"
#    "../../../JUCE/modules/juce_audio_basics/utilities/juce_LagrangeInterpolator.cpp"
    "../../../JUCE/modules/juce_audio_basics/utilities/juce_Reverb.h"
#    "../../../JUCE/modules/juce_audio_basics/utilities/juce_SmoothedValue.cpp"
    "../../../JUCE/modules/juce_audio_basics/utilities/juce_SmoothedValue.h"
#    "../../../JUCE/modules/juce_audio_basics/utilities/juce_WindowedSincInterpolator.cpp"
)
source_group("JUCE Modules\\juce_audio_basics\\utilities" FILES ${JUCE_Modules__juce_audio_basics__utilities})

set(JUCE_Modules__juce_audio_devices
#    "../../../JUCE/modules/juce_audio_devices/juce_audio_devices.cpp"
    "../../../JUCE/modules/juce_audio_devices/juce_audio_devices.h"
)
source_group("JUCE Modules\\juce_audio_devices" FILES ${JUCE_Modules__juce_audio_devices})

set(JUCE_Modules__juce_audio_devices__audio_io
#    "../../../JUCE/modules/juce_audio_devices/audio_io/juce_AudioDeviceManager.cpp"
    "../../../JUCE/modules/juce_audio_devices/audio_io/juce_AudioDeviceManager.h"
#    "../../../JUCE/modules/juce_audio_devices/audio_io/juce_AudioIODevice.cpp"
    "../../../JUCE/modules/juce_audio_devices/audio_io/juce_AudioIODevice.h"
#    "../../../JUCE/modules/juce_audio_devices/audio_io/juce_AudioIODeviceType.cpp"
    "../../../JUCE/modules/juce_audio_devices/audio_io/juce_AudioIODeviceType.h"
#    "../../../JUCE/modules/juce_audio_devices/audio_io/juce_SampleRateHelpers.cpp"
    "../../../JUCE/modules/juce_audio_devices/audio_io/juce_SystemAudioVolume.h"
)
source_group("JUCE Modules\\juce_audio_devices\\audio_io" FILES ${JUCE_Modules__juce_audio_devices__audio_io})

set(JUCE_Modules__juce_audio_devices__midi_io
#    "../../../JUCE/modules/juce_audio_devices/midi_io/juce_MidiDevices.cpp"
    "../../../JUCE/modules/juce_audio_devices/midi_io/juce_MidiDevices.h"
#    "../../../JUCE/modules/juce_audio_devices/midi_io/juce_MidiMessageCollector.cpp"
    "../../../JUCE/modules/juce_audio_devices/midi_io/juce_MidiMessageCollector.h"
)
source_group("JUCE Modules\\juce_audio_devices\\midi_io" FILES ${JUCE_Modules__juce_audio_devices__midi_io})

set(JUCE_Modules__juce_audio_devices__midi_io__ump
    "../../../JUCE/modules/juce_audio_devices/midi_io/ump/juce_UMPBytestreamInputHandler.h"
    "../../../JUCE/modules/juce_audio_devices/midi_io/ump/juce_UMPU32InputHandler.h"
)
source_group("JUCE Modules\\juce_audio_devices\\midi_io\\ump" FILES ${JUCE_Modules__juce_audio_devices__midi_io__ump})

set(JUCE_Modules__juce_audio_devices__native
#    "../../../JUCE/modules/juce_audio_devices/native/juce_android_Audio.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/juce_android_HighPerformanceAudioHelpers.h"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_android_Midi.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_android_Oboe.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_android_OpenSL.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_ios_Audio.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/juce_ios_Audio.h"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_linux_ALSA.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_linux_Bela.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_linux_JackAudio.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_linux_Midi.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_mac_CoreAudio.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_win32_ASIO.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_win32_DirectSound.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_win32_Midi.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/juce_win32_WASAPI.cpp"
)
source_group("JUCE Modules\\juce_audio_devices\\native" FILES ${JUCE_Modules__juce_audio_devices__native})

set(JUCE_Modules__juce_audio_devices__native__oboe
    "../../../JUCE/modules/juce_audio_devices/native/oboe/CMakeLists.txt"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/README.md"
)
source_group("JUCE Modules\\juce_audio_devices\\native\\oboe" FILES ${JUCE_Modules__juce_audio_devices__native__oboe})

set(JUCE_Modules__juce_audio_devices__native__oboe__include__oboe
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/AudioStream.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/AudioStreamBase.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/AudioStreamBuilder.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/AudioStreamCallback.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/Definitions.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/LatencyTuner.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/Oboe.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/ResultWithValue.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/StabilizedCallback.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/Utilities.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/include/oboe/Version.h"
)
source_group("JUCE Modules\\juce_audio_devices\\native\\oboe\\include\\oboe" FILES ${JUCE_Modules__juce_audio_devices__native__oboe__include__oboe})

set(JUCE_Modules__juce_audio_devices__native__oboe__src__aaudio
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/aaudio/AAudioExtensions.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/aaudio/AAudioLoader.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/aaudio/AAudioLoader.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/aaudio/AudioStreamAAudio.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/aaudio/AudioStreamAAudio.h"
)
source_group("JUCE Modules\\juce_audio_devices\\native\\oboe\\src\\aaudio" FILES ${JUCE_Modules__juce_audio_devices__native__oboe__src__aaudio})

set(JUCE_Modules__juce_audio_devices__native__oboe__src__common
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/AudioClock.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/AudioSourceCaller.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/AudioSourceCaller.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/AudioStream.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/AudioStreamBuilder.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/DataConversionFlowGraph.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/DataConversionFlowGraph.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/FilterAudioStream.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/FilterAudioStream.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/FixedBlockAdapter.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/FixedBlockAdapter.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/FixedBlockReader.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/FixedBlockReader.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/FixedBlockWriter.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/FixedBlockWriter.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/LatencyTuner.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/MonotonicCounter.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/OboeDebug.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/QuirksManager.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/QuirksManager.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/SourceFloatCaller.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/SourceFloatCaller.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/SourceI16Caller.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/SourceI16Caller.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/SourceI24Caller.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/SourceI24Caller.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/SourceI32Caller.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/SourceI32Caller.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/StabilizedCallback.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/Trace.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/Trace.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/Utilities.cpp"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/common/Version.cpp"
)
source_group("JUCE Modules\\juce_audio_devices\\native\\oboe\\src\\common" FILES ${JUCE_Modules__juce_audio_devices__native__oboe__src__common})

set(JUCE_Modules__juce_audio_devices__native__oboe__src__fifo
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/fifo/FifoBuffer.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/fifo/FifoBuffer.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/fifo/FifoController.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/fifo/FifoController.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/fifo/FifoControllerBase.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/fifo/FifoControllerBase.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/fifo/FifoControllerIndirect.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/fifo/FifoControllerIndirect.h"
)
source_group("JUCE Modules\\juce_audio_devices\\native\\oboe\\src\\fifo" FILES ${JUCE_Modules__juce_audio_devices__native__oboe__src__fifo})

set(JUCE_Modules__juce_audio_devices__native__oboe__src__flowgraph
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/ChannelCountConverter.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/ChannelCountConverter.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/ClipToRange.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/ClipToRange.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/FlowGraphNode.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/FlowGraphNode.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/FlowgraphUtilities.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/ManyToMultiConverter.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/ManyToMultiConverter.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/MonoToMultiConverter.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/MonoToMultiConverter.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/MultiToMonoConverter.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/MultiToMonoConverter.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/RampLinear.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/RampLinear.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SampleRateConverter.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SampleRateConverter.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SinkFloat.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SinkFloat.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SinkI16.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SinkI16.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SinkI24.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SinkI24.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SinkI32.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SinkI32.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SourceFloat.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SourceFloat.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SourceI16.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SourceI16.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SourceI24.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SourceI24.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SourceI32.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/SourceI32.h"
)
source_group("JUCE Modules\\juce_audio_devices\\native\\oboe\\src\\flowgraph" FILES ${JUCE_Modules__juce_audio_devices__native__oboe__src__flowgraph})

set(JUCE_Modules__juce_audio_devices__native__oboe__src__flowgraph__resampler
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/HyperbolicCosineWindow.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/IntegerRatio.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/IntegerRatio.h"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/KaiserWindow.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/LinearResampler.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/LinearResampler.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/MultiChannelResampler.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/MultiChannelResampler.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/PolyphaseResampler.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/PolyphaseResampler.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/PolyphaseResamplerMono.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/PolyphaseResamplerMono.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/PolyphaseResamplerStereo.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/PolyphaseResamplerStereo.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/SincResampler.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/SincResampler.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/SincResamplerStereo.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/flowgraph/resampler/SincResamplerStereo.h"
)
source_group("JUCE Modules\\juce_audio_devices\\native\\oboe\\src\\flowgraph\\resampler" FILES ${JUCE_Modules__juce_audio_devices__native__oboe__src__flowgraph__resampler})

set(JUCE_Modules__juce_audio_devices__native__oboe__src__opensles
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/AudioInputStreamOpenSLES.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/AudioInputStreamOpenSLES.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/AudioOutputStreamOpenSLES.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/AudioOutputStreamOpenSLES.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/AudioStreamBuffered.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/AudioStreamBuffered.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/AudioStreamOpenSLES.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/AudioStreamOpenSLES.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/EngineOpenSLES.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/EngineOpenSLES.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/OpenSLESUtilities.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/OpenSLESUtilities.h"
#    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/OutputMixerOpenSLES.cpp"
    "../../../JUCE/modules/juce_audio_devices/native/oboe/src/opensles/OutputMixerOpenSLES.h"
)
source_group("JUCE Modules\\juce_audio_devices\\native\\oboe\\src\\opensles" FILES ${JUCE_Modules__juce_audio_devices__native__oboe__src__opensles})

set(JUCE_Modules__juce_audio_devices__sources
#    "../../../JUCE/modules/juce_audio_devices/sources/juce_AudioSourcePlayer.cpp"
    "../../../JUCE/modules/juce_audio_devices/sources/juce_AudioSourcePlayer.h"
#    "../../../JUCE/modules/juce_audio_devices/sources/juce_AudioTransportSource.cpp"
    "../../../JUCE/modules/juce_audio_devices/sources/juce_AudioTransportSource.h"
)
source_group("JUCE Modules\\juce_audio_devices\\sources" FILES ${JUCE_Modules__juce_audio_devices__sources})

set(JUCE_Modules__juce_audio_formats
#    "../../../JUCE/modules/juce_audio_formats/juce_audio_formats.cpp"
    "../../../JUCE/modules/juce_audio_formats/juce_audio_formats.h"
)
source_group("JUCE Modules\\juce_audio_formats" FILES ${JUCE_Modules__juce_audio_formats})

set(JUCE_Modules__juce_audio_formats__codecs
#    "../../../JUCE/modules/juce_audio_formats/codecs/juce_AiffAudioFormat.cpp"
    "../../../JUCE/modules/juce_audio_formats/codecs/juce_AiffAudioFormat.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/juce_CoreAudioFormat.cpp"
    "../../../JUCE/modules/juce_audio_formats/codecs/juce_CoreAudioFormat.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/juce_FlacAudioFormat.cpp"
    "../../../JUCE/modules/juce_audio_formats/codecs/juce_FlacAudioFormat.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/juce_LAMEEncoderAudioFormat.cpp"
    "../../../JUCE/modules/juce_audio_formats/codecs/juce_LAMEEncoderAudioFormat.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/juce_MP3AudioFormat.cpp"
    "../../../JUCE/modules/juce_audio_formats/codecs/juce_MP3AudioFormat.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/juce_OggVorbisAudioFormat.cpp"
    "../../../JUCE/modules/juce_audio_formats/codecs/juce_OggVorbisAudioFormat.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/juce_WavAudioFormat.cpp"
    "../../../JUCE/modules/juce_audio_formats/codecs/juce_WavAudioFormat.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/juce_WindowsMediaAudioFormat.cpp"
    "../../../JUCE/modules/juce_audio_formats/codecs/juce_WindowsMediaAudioFormat.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs" FILES ${JUCE_Modules__juce_audio_formats__codecs})

set(JUCE_Modules__juce_audio_formats__codecs__flac
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/all.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/alloc.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/assert.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/callback.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/compat.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/endswap.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/export.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/Flac Licence.txt"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/format.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/metadata.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/ordinals.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/stream_decoder.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/stream_encoder.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\flac" FILES ${JUCE_Modules__juce_audio_formats__codecs__flac})

set(JUCE_Modules__juce_audio_formats__codecs__flac__libFLAC
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/bitmath.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/bitreader.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/bitwriter.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/cpu.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/crc.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/fixed.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/float.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/format.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/lpc_flac.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/md5.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/memory.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/stream_decoder.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/stream_encoder.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/stream_encoder_framing.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/window_flac.c"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\flac\\libFLAC" FILES ${JUCE_Modules__juce_audio_formats__codecs__flac__libFLAC})

set(JUCE_Modules__juce_audio_formats__codecs__flac__libFLAC__include__private
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/all.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/bitmath.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/bitreader.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/bitwriter.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/cpu.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/crc.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/fixed.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/float.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/format.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/lpc.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/md5.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/memory.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/metadata.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/stream_encoder.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/stream_encoder_framing.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/private/window.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\flac\\libFLAC\\include\\private" FILES ${JUCE_Modules__juce_audio_formats__codecs__flac__libFLAC__include__private})

set(JUCE_Modules__juce_audio_formats__codecs__flac__libFLAC__include__protected
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/protected/all.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/protected/stream_decoder.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/flac/libFLAC/include/protected/stream_encoder.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\flac\\libFLAC\\include\\protected" FILES ${JUCE_Modules__juce_audio_formats__codecs__flac__libFLAC__include__protected})

set(JUCE_Modules__juce_audio_formats__codecs__oggvorbis
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/bitwise.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/codec.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/config_types.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/crctable.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/framing.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/Ogg Vorbis Licence.txt"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/ogg.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/os_types.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/vorbisenc.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/vorbisfile.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\oggvorbis" FILES ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis})

set(JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/README.md"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\oggvorbis\\libvorbis-1.3.7" FILES ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7})

set(JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/analysis.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/backends.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/bitrate.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/bitrate.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/block.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/codebook.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/codebook.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/codec_internal.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/envelope.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/envelope.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/floor0.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/floor1.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/highlevel.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/info.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/lookup.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/lookup.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/lookup_data.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/lpc.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/lpc.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/lsp.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/lsp.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/mapping0.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/masking.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/mdct.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/mdct.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/misc.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/misc.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/os.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/psy.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/psy.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/registry.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/registry.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/res0.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/scales.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/sharedbook.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/smallft.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/smallft.h"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/synthesis.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/vorbisenc.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/vorbisfile.c"
#    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/window.c"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/window.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\oggvorbis\\libvorbis-1.3.7\\lib" FILES ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib})

set(JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__books__coupled
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/books/coupled/res_books_51.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/books/coupled/res_books_stereo.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\oggvorbis\\libvorbis-1.3.7\\lib\\books\\coupled" FILES ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__books__coupled})

set(JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__books__floor
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/books/floor/floor_books.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\oggvorbis\\libvorbis-1.3.7\\lib\\books\\floor" FILES ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__books__floor})

set(JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__books__uncoupled
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/books/uncoupled/res_books_uncoupled.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\oggvorbis\\libvorbis-1.3.7\\lib\\books\\uncoupled" FILES ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__books__uncoupled})

set(JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__modes
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/floor_all.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/psych_11.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/psych_16.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/psych_44.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/psych_8.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/residue_16.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/residue_44.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/residue_44p51.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/residue_44u.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/residue_8.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/setup_11.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/setup_16.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/setup_22.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/setup_32.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/setup_44.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/setup_44p51.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/setup_44u.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/setup_8.h"
    "../../../JUCE/modules/juce_audio_formats/codecs/oggvorbis/libvorbis-1.3.7/lib/modes/setup_X.h"
)
source_group("JUCE Modules\\juce_audio_formats\\codecs\\oggvorbis\\libvorbis-1.3.7\\lib\\modes" FILES ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__modes})

set(JUCE_Modules__juce_audio_formats__format
#    "../../../JUCE/modules/juce_audio_formats/format/juce_ARAAudioReaders.cpp"
    "../../../JUCE/modules/juce_audio_formats/format/juce_ARAAudioReaders.h"
#    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormat.cpp"
    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormat.h"
#    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormatManager.cpp"
    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormatManager.h"
#    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormatReader.cpp"
    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormatReader.h"
#    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormatReaderSource.cpp"
    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormatReaderSource.h"
#    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormatWriter.cpp"
    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioFormatWriter.h"
#    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioSubsectionReader.cpp"
    "../../../JUCE/modules/juce_audio_formats/format/juce_AudioSubsectionReader.h"
#    "../../../JUCE/modules/juce_audio_formats/format/juce_BufferingAudioFormatReader.cpp"
    "../../../JUCE/modules/juce_audio_formats/format/juce_BufferingAudioFormatReader.h"
    "../../../JUCE/modules/juce_audio_formats/format/juce_MemoryMappedAudioFormatReader.h"
)
source_group("JUCE Modules\\juce_audio_formats\\format" FILES ${JUCE_Modules__juce_audio_formats__format})

set(JUCE_Modules__juce_audio_formats__sampler
#    "../../../JUCE/modules/juce_audio_formats/sampler/juce_Sampler.cpp"
    "../../../JUCE/modules/juce_audio_formats/sampler/juce_Sampler.h"
)
source_group("JUCE Modules\\juce_audio_formats\\sampler" FILES ${JUCE_Modules__juce_audio_formats__sampler})

set(JUCE_Modules__juce_audio_plugin_client
    "../../../JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client.h"
#    "../../../JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_ARA.cpp"
#    "../../../JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp"
)
source_group("JUCE Modules\\juce_audio_plugin_client" FILES ${JUCE_Modules__juce_audio_plugin_client})

set(JUCE_Modules__juce_audio_plugin_client__ARA
#    "../../../JUCE/modules/juce_audio_plugin_client/ARA/juce_ARA_Wrapper.cpp"
)
source_group("JUCE Modules\\juce_audio_plugin_client\\ARA" FILES ${JUCE_Modules__juce_audio_plugin_client__ARA})

set(JUCE_Modules__juce_audio_plugin_client__utility
    "../../../JUCE/modules/juce_audio_plugin_client/utility/juce_CheckSettingMacros.h"
    "../../../JUCE/modules/juce_audio_plugin_client/utility/juce_CreatePluginFilter.h"
    "../../../JUCE/modules/juce_audio_plugin_client/utility/juce_IncludeModuleHeaders.h"
    "../../../JUCE/modules/juce_audio_plugin_client/utility/juce_IncludeSystemHeaders.h"
    "../../../JUCE/modules/juce_audio_plugin_client/utility/juce_LinuxMessageThread.h"
#    "../../../JUCE/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp"
    "../../../JUCE/modules/juce_audio_plugin_client/utility/juce_WindowsHooks.h"
)
source_group("JUCE Modules\\juce_audio_plugin_client\\utility" FILES ${JUCE_Modules__juce_audio_plugin_client__utility})

set(JUCE_Modules__juce_audio_processors
#    "../../../JUCE/modules/juce_audio_processors/juce_audio_processors.cpp"
    "../../../JUCE/modules/juce_audio_processors/juce_audio_processors.h"
#    "../../../JUCE/modules/juce_audio_processors/juce_audio_processors_ara.cpp"
#    "../../../JUCE/modules/juce_audio_processors/juce_audio_processors_lv2_libs.cpp"
)
source_group("JUCE Modules\\juce_audio_processors" FILES ${JUCE_Modules__juce_audio_processors})

set(JUCE_Modules__juce_audio_processors__format
#    "../../../JUCE/modules/juce_audio_processors/format/juce_AudioPluginFormat.cpp"
    "../../../JUCE/modules/juce_audio_processors/format/juce_AudioPluginFormat.h"
#    "../../../JUCE/modules/juce_audio_processors/format/juce_AudioPluginFormatManager.cpp"
    "../../../JUCE/modules/juce_audio_processors/format/juce_AudioPluginFormatManager.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format" FILES ${JUCE_Modules__juce_audio_processors__format})

set(JUCE_Modules__juce_audio_processors__format_types
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_ARACommon.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_ARACommon.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_ARAHosting.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_ARAHosting.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_AU_Shared.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_AudioUnitPluginFormat.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_LADSPAPluginFormat.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_LADSPAPluginFormat.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_LegacyAudioParameter.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_LV2Common.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_LV2PluginFormat.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_LV2PluginFormat.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_LV2PluginFormat_test.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_LV2Resources.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_LV2SupportLibs.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_VST3Common.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_VST3Headers.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_VST3PluginFormat.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_VST3PluginFormat.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_VST3PluginFormat_test.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_VSTCommon.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_VSTMidiEventList.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/juce_VSTPluginFormat.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/juce_VSTPluginFormat.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types" FILES ${JUCE_Modules__juce_audio_processors__format_types})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/juce_lv2_config.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv_config.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/README.md"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd_config.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord_config.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lilv__lilv
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/lilv/lilv.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/lilv/lilvmm.hpp"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lilv\\lilv" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lilv__lilv})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lilv__src
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/collections.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/filesystem.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/filesystem.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/instance.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/lib.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/lilv_internal.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/node.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/plugin.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/pluginclass.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/port.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/query.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/scalepoint.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/state.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/ui.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/util.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/world.c"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lilv\\src" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lilv__src})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lilv__src__zix
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/zix/common.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/zix/tree.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src/zix/tree.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lilv\\src\\zix" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lilv__src__zix})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__atom
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/atom/atom-test-utils.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/atom/atom-test.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/atom/atom.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/atom/forge-overflow-test.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/atom/forge.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/atom/util.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\atom" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__atom})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__buf-size
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/buf-size/buf-size.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\buf-size" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__buf-size})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__core
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/core/attributes.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/core/lv2.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/core/lv2_util.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\core" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__core})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__data-access
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/data-access/data-access.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\data-access" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__data-access})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__dynmanifest
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/dynmanifest/dynmanifest.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\dynmanifest" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__dynmanifest})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__event
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/event/event-helpers.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/event/event.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\event" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__event})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__instance-access
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/instance-access/instance-access.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\instance-access" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__instance-access})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__log
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/log/log.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/log/logger.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\log" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__log})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__midi
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/midi/midi.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\midi" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__midi})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__morph
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/morph/morph.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\morph" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__morph})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__options
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/options/options.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\options" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__options})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__parameters
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/parameters/parameters.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\parameters" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__parameters})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__patch
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/patch/patch.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\patch" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__patch})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__port-groups
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/port-groups/port-groups.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\port-groups" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__port-groups})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__port-props
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/port-props/port-props.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\port-props" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__port-props})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__presets
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/presets/presets.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\presets" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__presets})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__resize-port
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/resize-port/resize-port.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\resize-port" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__resize-port})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__state
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/state/state.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\state" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__state})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__time
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/time/time.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\time" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__time})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__ui
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/ui/ui.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\ui" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__ui})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__units
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/units/units.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\units" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__units})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__uri-map
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/uri-map/uri-map.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\uri-map" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__uri-map})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__urid
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/urid/urid.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\urid" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__urid})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__worker
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2/lv2/worker/worker.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\lv2\\lv2\\worker" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__worker})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__serd__serd
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/serd/serd.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\serd\\serd" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__serd__serd})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__serd__src
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/attributes.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/base64.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/base64.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/byte_sink.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/byte_source.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/byte_source.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/env.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/n3.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/node.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/node.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/reader.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/reader.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/serd_config.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/serd_internal.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/serdi.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/stack.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/string.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/string_utils.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/system.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/system.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/uri.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/uri_utils.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd/src/writer.c"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\serd\\src" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__serd__src})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sord__sord
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/sord/sord.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/sord/sordmm.hpp"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\sord\\sord" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sord__sord})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sord__src
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/sord.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/sord_config.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/sord_internal.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/sord_test.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/sord_validate.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/sordi.c"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/sordmm_test.cpp"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/syntax.c"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\sord\\src" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sord__src})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sord__src__zix
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/zix/btree.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/zix/btree.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/zix/common.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/zix/digest.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/zix/digest.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/zix/hash.c"
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src/zix/hash.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\sord\\src\\zix" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sord__src__zix})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sratom__sratom
    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sratom/sratom/sratom.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\sratom\\sratom" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sratom__sratom})

set(JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sratom__src
#    "../../../JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sratom/src/sratom.c"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\LV2_SDK\\sratom\\src" FILES ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sratom__src})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/JUCE_README.md"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/LICENSE.txt"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/README.md"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/LICENSE.txt"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/README.md"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\base" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base__source
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/baseiids.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/classfactoryhelpers.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fbuffer.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fbuffer.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fdebug.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fdebug.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fobject.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fobject.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fstreamer.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fstreamer.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fstring.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/fstring.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/updatehandler.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/source/updatehandler.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\base\\source" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base__source})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base__thread__include
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/thread/include/flock.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\base\\thread\\include" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base__thread__include})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base__thread__source
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/base/thread/source/flock.cpp"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\base\\thread\\source" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base__thread__source})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/LICENSE.txt"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/README.md"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\pluginterfaces" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces__base
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/conststringtable.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/conststringtable.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/coreiids.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/falignpop.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/falignpush.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/fplatform.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/fstrdefs.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/ftypes.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/funknown.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/funknown.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/futils.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/fvariant.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/ibstream.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/icloneable.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/ipersistent.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/ipluginbase.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/istringresult.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/iupdatehandler.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/smartpointer.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/typesizecheck.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/ustring.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/base/ustring.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\pluginterfaces\\base" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces__base})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces__gui
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/gui/iplugview.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/gui/iplugviewcontentscalesupport.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\pluginterfaces\\gui" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces__gui})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces__vst
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstattributes.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstaudioprocessor.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstautomationstate.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstchannelcontextinfo.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstcomponent.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstcontextmenu.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivsteditcontroller.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstevents.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivsthostapplication.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstinterappaudio.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstmessage.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstmidicontrollers.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstmidilearn.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstnoteexpression.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstparameterchanges.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstparameterfunctionname.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstphysicalui.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstpluginterfacesupport.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstplugview.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstprefetchablesupport.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstprocesscontext.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstrepresentation.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivsttestplugprovider.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/ivstunits.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/vstpshpack4.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/vstspeaker.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst/vsttypes.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\pluginterfaces\\vst" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces__vst})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/LICENSE.txt"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/README.md"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\public.sdk" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk__source__common
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/common/memorystream.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/common/memorystream.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/common/pluginview.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/common/pluginview.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\public.sdk\\source\\common" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk__source__common})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk__source__vst
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstbus.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstbus.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstcomponent.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstcomponent.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstcomponentbase.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstcomponentbase.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vsteditcontroller.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vsteditcontroller.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstinitiids.cpp"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstparameters.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstparameters.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstpresetfile.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/vstpresetfile.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\public.sdk\\source\\vst" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk__source__vst})

set(JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk__source__vst__hosting
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/hosting/hostclasses.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/hosting/hostclasses.h"
#    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/hosting/pluginterfacesupport.cpp"
    "../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK/public.sdk/source/vst/hosting/pluginterfacesupport.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\VST3_SDK\\public.sdk\\source\\vst\\hosting" FILES ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk__source__vst__hosting})

set(JUCE_Modules__juce_audio_processors__format_types__pslextensions
    "../../../JUCE/modules/juce_audio_processors/format_types/pslextensions/ipslcontextinfo.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/pslextensions/ipsleditcontroller.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/pslextensions/ipslgainreduction.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/pslextensions/ipslhostcommands.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/pslextensions/ipslviewembedding.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/pslextensions/ipslviewscaling.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/pslextensions/pslauextensions.h"
    "../../../JUCE/modules/juce_audio_processors/format_types/pslextensions/pslvst2extensions.h"
)
source_group("JUCE Modules\\juce_audio_processors\\format_types\\pslextensions" FILES ${JUCE_Modules__juce_audio_processors__format_types__pslextensions})

set(JUCE_Modules__juce_audio_processors__processors
#    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioPluginInstance.cpp"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioPluginInstance.h"
#    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessor.cpp"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessor.h"
#    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorEditor.cpp"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorEditor.h"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorEditorHostContext.h"
#    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorGraph.cpp"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorGraph.h"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorListener.h"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorParameter.h"
#    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorParameterGroup.cpp"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorParameterGroup.h"
#    "../../../JUCE/modules/juce_audio_processors/processors/juce_GenericAudioProcessorEditor.cpp"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_GenericAudioProcessorEditor.h"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_HostedAudioProcessorParameter.h"
#    "../../../JUCE/modules/juce_audio_processors/processors/juce_PluginDescription.cpp"
    "../../../JUCE/modules/juce_audio_processors/processors/juce_PluginDescription.h"
)
source_group("JUCE Modules\\juce_audio_processors\\processors" FILES ${JUCE_Modules__juce_audio_processors__processors})

set(JUCE_Modules__juce_audio_processors__scanning
#    "../../../JUCE/modules/juce_audio_processors/scanning/juce_KnownPluginList.cpp"
    "../../../JUCE/modules/juce_audio_processors/scanning/juce_KnownPluginList.h"
#    "../../../JUCE/modules/juce_audio_processors/scanning/juce_PluginDirectoryScanner.cpp"
    "../../../JUCE/modules/juce_audio_processors/scanning/juce_PluginDirectoryScanner.h"
#    "../../../JUCE/modules/juce_audio_processors/scanning/juce_PluginListComponent.cpp"
    "../../../JUCE/modules/juce_audio_processors/scanning/juce_PluginListComponent.h"
)
source_group("JUCE Modules\\juce_audio_processors\\scanning" FILES ${JUCE_Modules__juce_audio_processors__scanning})

set(JUCE_Modules__juce_audio_processors__utilities
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioParameterBool.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioParameterBool.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioParameterChoice.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioParameterChoice.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioParameterFloat.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioParameterFloat.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioParameterInt.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioParameterInt.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioProcessorParameterWithID.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioProcessorParameterWithID.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioProcessorValueTreeState.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_AudioProcessorValueTreeState.h"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_ExtensionsVisitor.h"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_FlagCache.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_NativeScaleFactorNotifier.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_NativeScaleFactorNotifier.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_ParameterAttachments.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_ParameterAttachments.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_PluginHostType.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_PluginHostType.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_RangedAudioParameter.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_RangedAudioParameter.h"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_VST3ClientExtensions.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/juce_VSTCallbackHandler.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/juce_VSTCallbackHandler.h"
)
source_group("JUCE Modules\\juce_audio_processors\\utilities" FILES ${JUCE_Modules__juce_audio_processors__utilities})

set(JUCE_Modules__juce_audio_processors__utilities__ARA
#    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARA_utils.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARA_utils.h"
    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARADebug.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARADocumentController.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARADocumentController.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARADocumentControllerCommon.cpp"
#    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARAModelObjects.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARAModelObjects.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARAPlugInInstanceRoles.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_ARAPlugInInstanceRoles.h"
#    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_AudioProcessor_ARAExtensions.cpp"
    "../../../JUCE/modules/juce_audio_processors/utilities/ARA/juce_AudioProcessor_ARAExtensions.h"
)
source_group("JUCE Modules\\juce_audio_processors\\utilities\\ARA" FILES ${JUCE_Modules__juce_audio_processors__utilities__ARA})

set(JUCE_Modules__juce_audio_utils
#    "../../../JUCE/modules/juce_audio_utils/juce_audio_utils.cpp"
    "../../../JUCE/modules/juce_audio_utils/juce_audio_utils.h"
)
source_group("JUCE Modules\\juce_audio_utils" FILES ${JUCE_Modules__juce_audio_utils})

set(JUCE_Modules__juce_audio_utils__audio_cd
    "../../../JUCE/modules/juce_audio_utils/audio_cd/juce_AudioCDBurner.h"
#    "../../../JUCE/modules/juce_audio_utils/audio_cd/juce_AudioCDReader.cpp"
    "../../../JUCE/modules/juce_audio_utils/audio_cd/juce_AudioCDReader.h"
)
source_group("JUCE Modules\\juce_audio_utils\\audio_cd" FILES ${JUCE_Modules__juce_audio_utils__audio_cd})

set(JUCE_Modules__juce_audio_utils__gui
#    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioAppComponent.cpp"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioAppComponent.h"
#    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioDeviceSelectorComponent.cpp"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioDeviceSelectorComponent.h"
#    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioThumbnail.cpp"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioThumbnail.h"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioThumbnailBase.h"
#    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioThumbnailCache.cpp"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioThumbnailCache.h"
#    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioVisualiserComponent.cpp"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_AudioVisualiserComponent.h"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_BluetoothMidiDevicePairingDialogue.h"
#    "../../../JUCE/modules/juce_audio_utils/gui/juce_KeyboardComponentBase.cpp"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_KeyboardComponentBase.h"
#    "../../../JUCE/modules/juce_audio_utils/gui/juce_MidiKeyboardComponent.cpp"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_MidiKeyboardComponent.h"
#    "../../../JUCE/modules/juce_audio_utils/gui/juce_MPEKeyboardComponent.cpp"
    "../../../JUCE/modules/juce_audio_utils/gui/juce_MPEKeyboardComponent.h"
)
source_group("JUCE Modules\\juce_audio_utils\\gui" FILES ${JUCE_Modules__juce_audio_utils__gui})

set(JUCE_Modules__juce_audio_utils__native
#    "../../../JUCE/modules/juce_audio_utils/native/juce_android_BluetoothMidiDevicePairingDialogue.cpp"
#    "../../../JUCE/modules/juce_audio_utils/native/juce_linux_AudioCDReader.cpp"
#    "../../../JUCE/modules/juce_audio_utils/native/juce_linux_BluetoothMidiDevicePairingDialogue.cpp"
#    "../../../JUCE/modules/juce_audio_utils/native/juce_win32_AudioCDBurner.cpp"
#    "../../../JUCE/modules/juce_audio_utils/native/juce_win32_AudioCDReader.cpp"
#    "../../../JUCE/modules/juce_audio_utils/native/juce_win_BluetoothMidiDevicePairingDialogue.cpp"
)
source_group("JUCE Modules\\juce_audio_utils\\native" FILES ${JUCE_Modules__juce_audio_utils__native})

set(JUCE_Modules__juce_audio_utils__players
#    "../../../JUCE/modules/juce_audio_utils/players/juce_AudioProcessorPlayer.cpp"
    "../../../JUCE/modules/juce_audio_utils/players/juce_AudioProcessorPlayer.h"
#    "../../../JUCE/modules/juce_audio_utils/players/juce_SoundPlayer.cpp"
    "../../../JUCE/modules/juce_audio_utils/players/juce_SoundPlayer.h"
)
source_group("JUCE Modules\\juce_audio_utils\\players" FILES ${JUCE_Modules__juce_audio_utils__players})

set(JUCE_Modules__juce_core
#    "../../../JUCE/modules/juce_core/juce_core.cpp"
    "../../../JUCE/modules/juce_core/juce_core.h"
)
source_group("JUCE Modules\\juce_core" FILES ${JUCE_Modules__juce_core})

set(JUCE_Modules__juce_core__containers
#    "../../../JUCE/modules/juce_core/containers/juce_AbstractFifo.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_AbstractFifo.h"
    "../../../JUCE/modules/juce_core/containers/juce_Array.h"
    "../../../JUCE/modules/juce_core/containers/juce_ArrayAllocationBase.h"
#    "../../../JUCE/modules/juce_core/containers/juce_ArrayBase.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_ArrayBase.h"
#    "../../../JUCE/modules/juce_core/containers/juce_DynamicObject.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_DynamicObject.h"
    "../../../JUCE/modules/juce_core/containers/juce_ElementComparator.h"
    "../../../JUCE/modules/juce_core/containers/juce_HashMap.h"
#    "../../../JUCE/modules/juce_core/containers/juce_HashMap_test.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_LinkedListPointer.h"
#    "../../../JUCE/modules/juce_core/containers/juce_ListenerList.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_ListenerList.h"
#    "../../../JUCE/modules/juce_core/containers/juce_NamedValueSet.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_NamedValueSet.h"
    "../../../JUCE/modules/juce_core/containers/juce_Optional.h"
#    "../../../JUCE/modules/juce_core/containers/juce_Optional_test.cpp"
#    "../../../JUCE/modules/juce_core/containers/juce_OwnedArray.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_OwnedArray.h"
#    "../../../JUCE/modules/juce_core/containers/juce_PropertySet.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_PropertySet.h"
#    "../../../JUCE/modules/juce_core/containers/juce_ReferenceCountedArray.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_ReferenceCountedArray.h"
    "../../../JUCE/modules/juce_core/containers/juce_ScopedValueSetter.h"
    "../../../JUCE/modules/juce_core/containers/juce_SingleThreadedAbstractFifo.h"
    "../../../JUCE/modules/juce_core/containers/juce_SortedSet.h"
#    "../../../JUCE/modules/juce_core/containers/juce_SparseSet.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_SparseSet.h"
#    "../../../JUCE/modules/juce_core/containers/juce_Variant.cpp"
    "../../../JUCE/modules/juce_core/containers/juce_Variant.h"
)
source_group("JUCE Modules\\juce_core\\containers" FILES ${JUCE_Modules__juce_core__containers})

set(JUCE_Modules__juce_core__files
    "../../../JUCE/modules/juce_core/files/juce_AndroidDocument.h"
#    "../../../JUCE/modules/juce_core/files/juce_common_MimeTypes.cpp"
    "../../../JUCE/modules/juce_core/files/juce_common_MimeTypes.h"
#    "../../../JUCE/modules/juce_core/files/juce_DirectoryIterator.cpp"
    "../../../JUCE/modules/juce_core/files/juce_DirectoryIterator.h"
#    "../../../JUCE/modules/juce_core/files/juce_File.cpp"
    "../../../JUCE/modules/juce_core/files/juce_File.h"
#    "../../../JUCE/modules/juce_core/files/juce_FileFilter.cpp"
    "../../../JUCE/modules/juce_core/files/juce_FileFilter.h"
#    "../../../JUCE/modules/juce_core/files/juce_FileInputStream.cpp"
    "../../../JUCE/modules/juce_core/files/juce_FileInputStream.h"
#    "../../../JUCE/modules/juce_core/files/juce_FileOutputStream.cpp"
    "../../../JUCE/modules/juce_core/files/juce_FileOutputStream.h"
#    "../../../JUCE/modules/juce_core/files/juce_FileSearchPath.cpp"
    "../../../JUCE/modules/juce_core/files/juce_FileSearchPath.h"
    "../../../JUCE/modules/juce_core/files/juce_MemoryMappedFile.h"
#    "../../../JUCE/modules/juce_core/files/juce_RangedDirectoryIterator.cpp"
    "../../../JUCE/modules/juce_core/files/juce_RangedDirectoryIterator.h"
#    "../../../JUCE/modules/juce_core/files/juce_TemporaryFile.cpp"
    "../../../JUCE/modules/juce_core/files/juce_TemporaryFile.h"
#    "../../../JUCE/modules/juce_core/files/juce_WildcardFileFilter.cpp"
    "../../../JUCE/modules/juce_core/files/juce_WildcardFileFilter.h"
)
source_group("JUCE Modules\\juce_core\\files" FILES ${JUCE_Modules__juce_core__files})

set(JUCE_Modules__juce_core__javascript
#    "../../../JUCE/modules/juce_core/javascript/juce_Javascript.cpp"
    "../../../JUCE/modules/juce_core/javascript/juce_Javascript.h"
#    "../../../JUCE/modules/juce_core/javascript/juce_JSON.cpp"
    "../../../JUCE/modules/juce_core/javascript/juce_JSON.h"
)
source_group("JUCE Modules\\juce_core\\javascript" FILES ${JUCE_Modules__juce_core__javascript})

set(JUCE_Modules__juce_core__logging
#    "../../../JUCE/modules/juce_core/logging/juce_FileLogger.cpp"
    "../../../JUCE/modules/juce_core/logging/juce_FileLogger.h"
#    "../../../JUCE/modules/juce_core/logging/juce_Logger.cpp"
    "../../../JUCE/modules/juce_core/logging/juce_Logger.h"
)
source_group("JUCE Modules\\juce_core\\logging" FILES ${JUCE_Modules__juce_core__logging})

set(JUCE_Modules__juce_core__maths
#    "../../../JUCE/modules/juce_core/maths/juce_BigInteger.cpp"
    "../../../JUCE/modules/juce_core/maths/juce_BigInteger.h"
#    "../../../JUCE/modules/juce_core/maths/juce_Expression.cpp"
    "../../../JUCE/modules/juce_core/maths/juce_Expression.h"
    "../../../JUCE/modules/juce_core/maths/juce_MathsFunctions.h"
    "../../../JUCE/modules/juce_core/maths/juce_NormalisableRange.h"
#    "../../../JUCE/modules/juce_core/maths/juce_Random.cpp"
    "../../../JUCE/modules/juce_core/maths/juce_Random.h"
    "../../../JUCE/modules/juce_core/maths/juce_Range.h"
    "../../../JUCE/modules/juce_core/maths/juce_StatisticsAccumulator.h"
)
source_group("JUCE Modules\\juce_core\\maths" FILES ${JUCE_Modules__juce_core__maths})

set(JUCE_Modules__juce_core__memory
#    "../../../JUCE/modules/juce_core/memory/juce_AllocationHooks.cpp"
    "../../../JUCE/modules/juce_core/memory/juce_AllocationHooks.h"
    "../../../JUCE/modules/juce_core/memory/juce_Atomic.h"
    "../../../JUCE/modules/juce_core/memory/juce_ByteOrder.h"
    "../../../JUCE/modules/juce_core/memory/juce_ContainerDeletePolicy.h"
    "../../../JUCE/modules/juce_core/memory/juce_HeapBlock.h"
    "../../../JUCE/modules/juce_core/memory/juce_HeavyweightLeakedObjectDetector.h"
    "../../../JUCE/modules/juce_core/memory/juce_LeakedObjectDetector.h"
    "../../../JUCE/modules/juce_core/memory/juce_Memory.h"
#    "../../../JUCE/modules/juce_core/memory/juce_MemoryBlock.cpp"
    "../../../JUCE/modules/juce_core/memory/juce_MemoryBlock.h"
    "../../../JUCE/modules/juce_core/memory/juce_OptionalScopedPointer.h"
    "../../../JUCE/modules/juce_core/memory/juce_ReferenceCountedObject.h"
    "../../../JUCE/modules/juce_core/memory/juce_Reservoir.h"
    "../../../JUCE/modules/juce_core/memory/juce_ScopedPointer.h"
    "../../../JUCE/modules/juce_core/memory/juce_SharedResourcePointer.h"
    "../../../JUCE/modules/juce_core/memory/juce_Singleton.h"
    "../../../JUCE/modules/juce_core/memory/juce_WeakReference.h"
)
source_group("JUCE Modules\\juce_core\\memory" FILES ${JUCE_Modules__juce_core__memory})

set(JUCE_Modules__juce_core__misc
#    "../../../JUCE/modules/juce_core/misc/juce_ConsoleApplication.cpp"
    "../../../JUCE/modules/juce_core/misc/juce_ConsoleApplication.h"
    "../../../JUCE/modules/juce_core/misc/juce_Functional.h"
#    "../../../JUCE/modules/juce_core/misc/juce_Result.cpp"
    "../../../JUCE/modules/juce_core/misc/juce_Result.h"
#    "../../../JUCE/modules/juce_core/misc/juce_RuntimePermissions.cpp"
    "../../../JUCE/modules/juce_core/misc/juce_RuntimePermissions.h"
#    "../../../JUCE/modules/juce_core/misc/juce_Uuid.cpp"
    "../../../JUCE/modules/juce_core/misc/juce_Uuid.h"
    "../../../JUCE/modules/juce_core/misc/juce_WindowsRegistry.h"
)
source_group("JUCE Modules\\juce_core\\misc" FILES ${JUCE_Modules__juce_core__misc})

set(JUCE_Modules__juce_core__native
#    "../../../JUCE/modules/juce_core/native/juce_android_AndroidDocument.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_android_Files.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_android_JNIHelpers.cpp"
    "../../../JUCE/modules/juce_core/native/juce_android_JNIHelpers.h"
#    "../../../JUCE/modules/juce_core/native/juce_android_Misc.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_android_Network.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_android_RuntimePermissions.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_android_SystemStats.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_android_Threads.cpp"
    "../../../JUCE/modules/juce_core/native/juce_BasicNativeHeaders.h"
#    "../../../JUCE/modules/juce_core/native/juce_curl_Network.cpp"
    "../../../JUCE/modules/juce_core/native/juce_intel_SharedCode.h"
#    "../../../JUCE/modules/juce_core/native/juce_linux_CommonFile.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_linux_Files.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_linux_Network.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_linux_SystemStats.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_linux_Threads.cpp"
    "../../../JUCE/modules/juce_core/native/juce_mac_CFHelpers.h"
    "../../../JUCE/modules/juce_core/native/juce_mac_ObjCHelpers.h"
    "../../../JUCE/modules/juce_core/native/juce_native_ThreadPriorities.h"
    "../../../JUCE/modules/juce_core/native/juce_posix_IPAddress.h"
#    "../../../JUCE/modules/juce_core/native/juce_posix_NamedPipe.cpp"
    "../../../JUCE/modules/juce_core/native/juce_posix_SharedCode.h"
#    "../../../JUCE/modules/juce_core/native/juce_wasm_SystemStats.cpp"
    "../../../JUCE/modules/juce_core/native/juce_win32_ComSmartPtr.h"
#    "../../../JUCE/modules/juce_core/native/juce_win32_Files.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_win32_Network.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_win32_Registry.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_win32_SystemStats.cpp"
#    "../../../JUCE/modules/juce_core/native/juce_win32_Threads.cpp"
)
source_group("JUCE Modules\\juce_core\\native" FILES ${JUCE_Modules__juce_core__native})

set(JUCE_Modules__juce_core__native__java
    "../../../JUCE/modules/juce_core/native/java/README.txt"
)
source_group("JUCE Modules\\juce_core\\native\\java" FILES ${JUCE_Modules__juce_core__native__java})

set(JUCE_Modules__juce_core__network
#    "../../../JUCE/modules/juce_core/network/juce_IPAddress.cpp"
    "../../../JUCE/modules/juce_core/network/juce_IPAddress.h"
#    "../../../JUCE/modules/juce_core/network/juce_MACAddress.cpp"
    "../../../JUCE/modules/juce_core/network/juce_MACAddress.h"
#    "../../../JUCE/modules/juce_core/network/juce_NamedPipe.cpp"
    "../../../JUCE/modules/juce_core/network/juce_NamedPipe.h"
#    "../../../JUCE/modules/juce_core/network/juce_Socket.cpp"
    "../../../JUCE/modules/juce_core/network/juce_Socket.h"
#    "../../../JUCE/modules/juce_core/network/juce_URL.cpp"
    "../../../JUCE/modules/juce_core/network/juce_URL.h"
#    "../../../JUCE/modules/juce_core/network/juce_WebInputStream.cpp"
    "../../../JUCE/modules/juce_core/network/juce_WebInputStream.h"
)
source_group("JUCE Modules\\juce_core\\network" FILES ${JUCE_Modules__juce_core__network})

set(JUCE_Modules__juce_core__streams
    "../../../JUCE/modules/juce_core/streams/juce_AndroidDocumentInputSource.h"
#    "../../../JUCE/modules/juce_core/streams/juce_BufferedInputStream.cpp"
    "../../../JUCE/modules/juce_core/streams/juce_BufferedInputStream.h"
#    "../../../JUCE/modules/juce_core/streams/juce_FileInputSource.cpp"
    "../../../JUCE/modules/juce_core/streams/juce_FileInputSource.h"
    "../../../JUCE/modules/juce_core/streams/juce_InputSource.h"
#    "../../../JUCE/modules/juce_core/streams/juce_InputStream.cpp"
    "../../../JUCE/modules/juce_core/streams/juce_InputStream.h"
#    "../../../JUCE/modules/juce_core/streams/juce_MemoryInputStream.cpp"
    "../../../JUCE/modules/juce_core/streams/juce_MemoryInputStream.h"
#    "../../../JUCE/modules/juce_core/streams/juce_MemoryOutputStream.cpp"
    "../../../JUCE/modules/juce_core/streams/juce_MemoryOutputStream.h"
#    "../../../JUCE/modules/juce_core/streams/juce_OutputStream.cpp"
    "../../../JUCE/modules/juce_core/streams/juce_OutputStream.h"
#    "../../../JUCE/modules/juce_core/streams/juce_SubregionStream.cpp"
    "../../../JUCE/modules/juce_core/streams/juce_SubregionStream.h"
#    "../../../JUCE/modules/juce_core/streams/juce_URLInputSource.cpp"
    "../../../JUCE/modules/juce_core/streams/juce_URLInputSource.h"
)
source_group("JUCE Modules\\juce_core\\streams" FILES ${JUCE_Modules__juce_core__streams})

set(JUCE_Modules__juce_core__system
    "../../../JUCE/modules/juce_core/system/juce_CompilerSupport.h"
    "../../../JUCE/modules/juce_core/system/juce_CompilerWarnings.h"
    "../../../JUCE/modules/juce_core/system/juce_PlatformDefs.h"
    "../../../JUCE/modules/juce_core/system/juce_StandardHeader.h"
#    "../../../JUCE/modules/juce_core/system/juce_SystemStats.cpp"
    "../../../JUCE/modules/juce_core/system/juce_SystemStats.h"
    "../../../JUCE/modules/juce_core/system/juce_TargetPlatform.h"
)
source_group("JUCE Modules\\juce_core\\system" FILES ${JUCE_Modules__juce_core__system})

set(JUCE_Modules__juce_core__text
#    "../../../JUCE/modules/juce_core/text/juce_Base64.cpp"
    "../../../JUCE/modules/juce_core/text/juce_Base64.h"
#    "../../../JUCE/modules/juce_core/text/juce_CharacterFunctions.cpp"
    "../../../JUCE/modules/juce_core/text/juce_CharacterFunctions.h"
    "../../../JUCE/modules/juce_core/text/juce_CharPointer_ASCII.h"
    "../../../JUCE/modules/juce_core/text/juce_CharPointer_UTF16.h"
    "../../../JUCE/modules/juce_core/text/juce_CharPointer_UTF32.h"
    "../../../JUCE/modules/juce_core/text/juce_CharPointer_UTF8.h"
#    "../../../JUCE/modules/juce_core/text/juce_Identifier.cpp"
    "../../../JUCE/modules/juce_core/text/juce_Identifier.h"
#    "../../../JUCE/modules/juce_core/text/juce_LocalisedStrings.cpp"
    "../../../JUCE/modules/juce_core/text/juce_LocalisedStrings.h"
    "../../../JUCE/modules/juce_core/text/juce_NewLine.h"
#    "../../../JUCE/modules/juce_core/text/juce_String.cpp"
    "../../../JUCE/modules/juce_core/text/juce_String.h"
#    "../../../JUCE/modules/juce_core/text/juce_StringArray.cpp"
    "../../../JUCE/modules/juce_core/text/juce_StringArray.h"
#    "../../../JUCE/modules/juce_core/text/juce_StringPairArray.cpp"
    "../../../JUCE/modules/juce_core/text/juce_StringPairArray.h"
#    "../../../JUCE/modules/juce_core/text/juce_StringPool.cpp"
    "../../../JUCE/modules/juce_core/text/juce_StringPool.h"
    "../../../JUCE/modules/juce_core/text/juce_StringRef.h"
#    "../../../JUCE/modules/juce_core/text/juce_TextDiff.cpp"
    "../../../JUCE/modules/juce_core/text/juce_TextDiff.h"
)
source_group("JUCE Modules\\juce_core\\text" FILES ${JUCE_Modules__juce_core__text})

set(JUCE_Modules__juce_core__threads
#    "../../../JUCE/modules/juce_core/threads/juce_ChildProcess.cpp"
    "../../../JUCE/modules/juce_core/threads/juce_ChildProcess.h"
    "../../../JUCE/modules/juce_core/threads/juce_CriticalSection.h"
    "../../../JUCE/modules/juce_core/threads/juce_DynamicLibrary.h"
#    "../../../JUCE/modules/juce_core/threads/juce_HighResolutionTimer.cpp"
    "../../../JUCE/modules/juce_core/threads/juce_HighResolutionTimer.h"
    "../../../JUCE/modules/juce_core/threads/juce_InterProcessLock.h"
    "../../../JUCE/modules/juce_core/threads/juce_Process.h"
#    "../../../JUCE/modules/juce_core/threads/juce_ReadWriteLock.cpp"
    "../../../JUCE/modules/juce_core/threads/juce_ReadWriteLock.h"
    "../../../JUCE/modules/juce_core/threads/juce_ScopedLock.h"
    "../../../JUCE/modules/juce_core/threads/juce_ScopedReadLock.h"
    "../../../JUCE/modules/juce_core/threads/juce_ScopedWriteLock.h"
    "../../../JUCE/modules/juce_core/threads/juce_SpinLock.h"
#    "../../../JUCE/modules/juce_core/threads/juce_Thread.cpp"
    "../../../JUCE/modules/juce_core/threads/juce_Thread.h"
    "../../../JUCE/modules/juce_core/threads/juce_ThreadLocalValue.h"
#    "../../../JUCE/modules/juce_core/threads/juce_ThreadPool.cpp"
    "../../../JUCE/modules/juce_core/threads/juce_ThreadPool.h"
#    "../../../JUCE/modules/juce_core/threads/juce_TimeSliceThread.cpp"
    "../../../JUCE/modules/juce_core/threads/juce_TimeSliceThread.h"
#    "../../../JUCE/modules/juce_core/threads/juce_WaitableEvent.cpp"
    "../../../JUCE/modules/juce_core/threads/juce_WaitableEvent.h"
)
source_group("JUCE Modules\\juce_core\\threads" FILES ${JUCE_Modules__juce_core__threads})

set(JUCE_Modules__juce_core__time
#    "../../../JUCE/modules/juce_core/time/juce_PerformanceCounter.cpp"
    "../../../JUCE/modules/juce_core/time/juce_PerformanceCounter.h"
#    "../../../JUCE/modules/juce_core/time/juce_RelativeTime.cpp"
    "../../../JUCE/modules/juce_core/time/juce_RelativeTime.h"
#    "../../../JUCE/modules/juce_core/time/juce_Time.cpp"
    "../../../JUCE/modules/juce_core/time/juce_Time.h"
)
source_group("JUCE Modules\\juce_core\\time" FILES ${JUCE_Modules__juce_core__time})

set(JUCE_Modules__juce_core__unit_tests
#    "../../../JUCE/modules/juce_core/unit_tests/juce_UnitTest.cpp"
    "../../../JUCE/modules/juce_core/unit_tests/juce_UnitTest.h"
    "../../../JUCE/modules/juce_core/unit_tests/juce_UnitTestCategories.h"
)
source_group("JUCE Modules\\juce_core\\unit_tests" FILES ${JUCE_Modules__juce_core__unit_tests})

set(JUCE_Modules__juce_core__xml
#    "../../../JUCE/modules/juce_core/xml/juce_XmlDocument.cpp"
    "../../../JUCE/modules/juce_core/xml/juce_XmlDocument.h"
#    "../../../JUCE/modules/juce_core/xml/juce_XmlElement.cpp"
    "../../../JUCE/modules/juce_core/xml/juce_XmlElement.h"
)
source_group("JUCE Modules\\juce_core\\xml" FILES ${JUCE_Modules__juce_core__xml})

set(JUCE_Modules__juce_core__zip
#    "../../../JUCE/modules/juce_core/zip/juce_GZIPCompressorOutputStream.cpp"
    "../../../JUCE/modules/juce_core/zip/juce_GZIPCompressorOutputStream.h"
#    "../../../JUCE/modules/juce_core/zip/juce_GZIPDecompressorInputStream.cpp"
    "../../../JUCE/modules/juce_core/zip/juce_GZIPDecompressorInputStream.h"
#    "../../../JUCE/modules/juce_core/zip/juce_ZipFile.cpp"
    "../../../JUCE/modules/juce_core/zip/juce_ZipFile.h"
)
source_group("JUCE Modules\\juce_core\\zip" FILES ${JUCE_Modules__juce_core__zip})

set(JUCE_Modules__juce_core__zip__zlib
#    "../../../JUCE/modules/juce_core/zip/zlib/adler32.c"
#    "../../../JUCE/modules/juce_core/zip/zlib/compress.c"
#    "../../../JUCE/modules/juce_core/zip/zlib/crc32.c"
    "../../../JUCE/modules/juce_core/zip/zlib/crc32.h"
#    "../../../JUCE/modules/juce_core/zip/zlib/deflate.c"
    "../../../JUCE/modules/juce_core/zip/zlib/deflate.h"
#    "../../../JUCE/modules/juce_core/zip/zlib/infback.c"
#    "../../../JUCE/modules/juce_core/zip/zlib/inffast.c"
    "../../../JUCE/modules/juce_core/zip/zlib/inffast.h"
    "../../../JUCE/modules/juce_core/zip/zlib/inffixed.h"
#    "../../../JUCE/modules/juce_core/zip/zlib/inflate.c"
    "../../../JUCE/modules/juce_core/zip/zlib/inflate.h"
#    "../../../JUCE/modules/juce_core/zip/zlib/inftrees.c"
    "../../../JUCE/modules/juce_core/zip/zlib/inftrees.h"
#    "../../../JUCE/modules/juce_core/zip/zlib/trees.c"
    "../../../JUCE/modules/juce_core/zip/zlib/trees.h"
#    "../../../JUCE/modules/juce_core/zip/zlib/uncompr.c"
    "../../../JUCE/modules/juce_core/zip/zlib/zconf.h"
    "../../../JUCE/modules/juce_core/zip/zlib/zconf.in.h"
    "../../../JUCE/modules/juce_core/zip/zlib/zlib.h"
#    "../../../JUCE/modules/juce_core/zip/zlib/zutil.c"
    "../../../JUCE/modules/juce_core/zip/zlib/zutil.h"
)
source_group("JUCE Modules\\juce_core\\zip\\zlib" FILES ${JUCE_Modules__juce_core__zip__zlib})

set(JUCE_Modules__juce_data_structures
#    "../../../JUCE/modules/juce_data_structures/juce_data_structures.cpp"
    "../../../JUCE/modules/juce_data_structures/juce_data_structures.h"
)
source_group("JUCE Modules\\juce_data_structures" FILES ${JUCE_Modules__juce_data_structures})

set(JUCE_Modules__juce_data_structures__app_properties
#    "../../../JUCE/modules/juce_data_structures/app_properties/juce_ApplicationProperties.cpp"
    "../../../JUCE/modules/juce_data_structures/app_properties/juce_ApplicationProperties.h"
#    "../../../JUCE/modules/juce_data_structures/app_properties/juce_PropertiesFile.cpp"
    "../../../JUCE/modules/juce_data_structures/app_properties/juce_PropertiesFile.h"
)
source_group("JUCE Modules\\juce_data_structures\\app_properties" FILES ${JUCE_Modules__juce_data_structures__app_properties})

set(JUCE_Modules__juce_data_structures__undomanager
#    "../../../JUCE/modules/juce_data_structures/undomanager/juce_UndoableAction.cpp"
    "../../../JUCE/modules/juce_data_structures/undomanager/juce_UndoableAction.h"
#    "../../../JUCE/modules/juce_data_structures/undomanager/juce_UndoManager.cpp"
    "../../../JUCE/modules/juce_data_structures/undomanager/juce_UndoManager.h"
)
source_group("JUCE Modules\\juce_data_structures\\undomanager" FILES ${JUCE_Modules__juce_data_structures__undomanager})

set(JUCE_Modules__juce_data_structures__values
#    "../../../JUCE/modules/juce_data_structures/values/juce_CachedValue.cpp"
    "../../../JUCE/modules/juce_data_structures/values/juce_CachedValue.h"
#    "../../../JUCE/modules/juce_data_structures/values/juce_Value.cpp"
    "../../../JUCE/modules/juce_data_structures/values/juce_Value.h"
#    "../../../JUCE/modules/juce_data_structures/values/juce_ValueTree.cpp"
    "../../../JUCE/modules/juce_data_structures/values/juce_ValueTree.h"
    "../../../JUCE/modules/juce_data_structures/values/juce_ValueTreePropertyWithDefault.h"
#    "../../../JUCE/modules/juce_data_structures/values/juce_ValueTreePropertyWithDefault_test.cpp"
#    "../../../JUCE/modules/juce_data_structures/values/juce_ValueTreeSynchroniser.cpp"
    "../../../JUCE/modules/juce_data_structures/values/juce_ValueTreeSynchroniser.h"
)
source_group("JUCE Modules\\juce_data_structures\\values" FILES ${JUCE_Modules__juce_data_structures__values})

set(JUCE_Modules__juce_dsp
#    "../../../JUCE/modules/juce_dsp/juce_dsp.cpp"
    "../../../JUCE/modules/juce_dsp/juce_dsp.h"
)
source_group("JUCE Modules\\juce_dsp" FILES ${JUCE_Modules__juce_dsp})

set(JUCE_Modules__juce_dsp__containers
    "../../../JUCE/modules/juce_dsp/containers/juce_AudioBlock.h"
#    "../../../JUCE/modules/juce_dsp/containers/juce_AudioBlock_test.cpp"
    "../../../JUCE/modules/juce_dsp/containers/juce_FixedSizeFunction.h"
#    "../../../JUCE/modules/juce_dsp/containers/juce_FixedSizeFunction_test.cpp"
    "../../../JUCE/modules/juce_dsp/containers/juce_SIMDRegister.h"
    "../../../JUCE/modules/juce_dsp/containers/juce_SIMDRegister_Impl.h"
#    "../../../JUCE/modules/juce_dsp/containers/juce_SIMDRegister_test.cpp"
)
source_group("JUCE Modules\\juce_dsp\\containers" FILES ${JUCE_Modules__juce_dsp__containers})

set(JUCE_Modules__juce_dsp__filter_design
#    "../../../JUCE/modules/juce_dsp/filter_design/juce_FilterDesign.cpp"
    "../../../JUCE/modules/juce_dsp/filter_design/juce_FilterDesign.h"
)
source_group("JUCE Modules\\juce_dsp\\filter_design" FILES ${JUCE_Modules__juce_dsp__filter_design})

set(JUCE_Modules__juce_dsp__frequency
#    "../../../JUCE/modules/juce_dsp/frequency/juce_Convolution.cpp"
    "../../../JUCE/modules/juce_dsp/frequency/juce_Convolution.h"
#    "../../../JUCE/modules/juce_dsp/frequency/juce_Convolution_test.cpp"
#    "../../../JUCE/modules/juce_dsp/frequency/juce_FFT.cpp"
    "../../../JUCE/modules/juce_dsp/frequency/juce_FFT.h"
#    "../../../JUCE/modules/juce_dsp/frequency/juce_FFT_test.cpp"
#    "../../../JUCE/modules/juce_dsp/frequency/juce_Windowing.cpp"
    "../../../JUCE/modules/juce_dsp/frequency/juce_Windowing.h"
)
source_group("JUCE Modules\\juce_dsp\\frequency" FILES ${JUCE_Modules__juce_dsp__frequency})

set(JUCE_Modules__juce_dsp__maths
    "../../../JUCE/modules/juce_dsp/maths/juce_FastMathApproximations.h"
    "../../../JUCE/modules/juce_dsp/maths/juce_LogRampedValue.h"
#    "../../../JUCE/modules/juce_dsp/maths/juce_LogRampedValue_test.cpp"
#    "../../../JUCE/modules/juce_dsp/maths/juce_LookupTable.cpp"
    "../../../JUCE/modules/juce_dsp/maths/juce_LookupTable.h"
#    "../../../JUCE/modules/juce_dsp/maths/juce_Matrix.cpp"
    "../../../JUCE/modules/juce_dsp/maths/juce_Matrix.h"
#    "../../../JUCE/modules/juce_dsp/maths/juce_Matrix_test.cpp"
    "../../../JUCE/modules/juce_dsp/maths/juce_Phase.h"
    "../../../JUCE/modules/juce_dsp/maths/juce_Polynomial.h"
#    "../../../JUCE/modules/juce_dsp/maths/juce_SpecialFunctions.cpp"
    "../../../JUCE/modules/juce_dsp/maths/juce_SpecialFunctions.h"
)
source_group("JUCE Modules\\juce_dsp\\maths" FILES ${JUCE_Modules__juce_dsp__maths})

set(JUCE_Modules__juce_dsp__native
#    "../../../JUCE/modules/juce_dsp/native/juce_avx_SIMDNativeOps.cpp"
    "../../../JUCE/modules/juce_dsp/native/juce_avx_SIMDNativeOps.h"
    "../../../JUCE/modules/juce_dsp/native/juce_fallback_SIMDNativeOps.h"
#    "../../../JUCE/modules/juce_dsp/native/juce_neon_SIMDNativeOps.cpp"
    "../../../JUCE/modules/juce_dsp/native/juce_neon_SIMDNativeOps.h"
#    "../../../JUCE/modules/juce_dsp/native/juce_sse_SIMDNativeOps.cpp"
    "../../../JUCE/modules/juce_dsp/native/juce_sse_SIMDNativeOps.h"
)
source_group("JUCE Modules\\juce_dsp\\native" FILES ${JUCE_Modules__juce_dsp__native})

set(JUCE_Modules__juce_dsp__processors
#    "../../../JUCE/modules/juce_dsp/processors/juce_BallisticsFilter.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_BallisticsFilter.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_DelayLine.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_DelayLine.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_DryWetMixer.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_DryWetMixer.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_FIRFilter.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_FIRFilter.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_FIRFilter_test.cpp"
#    "../../../JUCE/modules/juce_dsp/processors/juce_FirstOrderTPTFilter.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_FirstOrderTPTFilter.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_IIRFilter.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_IIRFilter.h"
    "../../../JUCE/modules/juce_dsp/processors/juce_IIRFilter_Impl.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_LinkwitzRileyFilter.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_LinkwitzRileyFilter.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_Oversampling.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_Oversampling.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_Panner.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_Panner.h"
    "../../../JUCE/modules/juce_dsp/processors/juce_ProcessContext.h"
    "../../../JUCE/modules/juce_dsp/processors/juce_ProcessorChain.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_ProcessorChain_test.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_ProcessorDuplicator.h"
    "../../../JUCE/modules/juce_dsp/processors/juce_ProcessorWrapper.h"
    "../../../JUCE/modules/juce_dsp/processors/juce_StateVariableFilter.h"
#    "../../../JUCE/modules/juce_dsp/processors/juce_StateVariableTPTFilter.cpp"
    "../../../JUCE/modules/juce_dsp/processors/juce_StateVariableTPTFilter.h"
)
source_group("JUCE Modules\\juce_dsp\\processors" FILES ${JUCE_Modules__juce_dsp__processors})

set(JUCE_Modules__juce_dsp__widgets
    "../../../JUCE/modules/juce_dsp/widgets/juce_Bias.h"
#    "../../../JUCE/modules/juce_dsp/widgets/juce_Chorus.cpp"
    "../../../JUCE/modules/juce_dsp/widgets/juce_Chorus.h"
#    "../../../JUCE/modules/juce_dsp/widgets/juce_Compressor.cpp"
    "../../../JUCE/modules/juce_dsp/widgets/juce_Compressor.h"
    "../../../JUCE/modules/juce_dsp/widgets/juce_Gain.h"
#    "../../../JUCE/modules/juce_dsp/widgets/juce_LadderFilter.cpp"
    "../../../JUCE/modules/juce_dsp/widgets/juce_LadderFilter.h"
#    "../../../JUCE/modules/juce_dsp/widgets/juce_Limiter.cpp"
    "../../../JUCE/modules/juce_dsp/widgets/juce_Limiter.h"
#    "../../../JUCE/modules/juce_dsp/widgets/juce_NoiseGate.cpp"
    "../../../JUCE/modules/juce_dsp/widgets/juce_NoiseGate.h"
    "../../../JUCE/modules/juce_dsp/widgets/juce_Oscillator.h"
#    "../../../JUCE/modules/juce_dsp/widgets/juce_Phaser.cpp"
    "../../../JUCE/modules/juce_dsp/widgets/juce_Phaser.h"
    "../../../JUCE/modules/juce_dsp/widgets/juce_Reverb.h"
    "../../../JUCE/modules/juce_dsp/widgets/juce_WaveShaper.h"
)
source_group("JUCE Modules\\juce_dsp\\widgets" FILES ${JUCE_Modules__juce_dsp__widgets})

set(JUCE_Modules__juce_events
#    "../../../JUCE/modules/juce_events/juce_events.cpp"
    "../../../JUCE/modules/juce_events/juce_events.h"
)
source_group("JUCE Modules\\juce_events" FILES ${JUCE_Modules__juce_events})

set(JUCE_Modules__juce_events__broadcasters
#    "../../../JUCE/modules/juce_events/broadcasters/juce_ActionBroadcaster.cpp"
    "../../../JUCE/modules/juce_events/broadcasters/juce_ActionBroadcaster.h"
    "../../../JUCE/modules/juce_events/broadcasters/juce_ActionListener.h"
#    "../../../JUCE/modules/juce_events/broadcasters/juce_AsyncUpdater.cpp"
    "../../../JUCE/modules/juce_events/broadcasters/juce_AsyncUpdater.h"
#    "../../../JUCE/modules/juce_events/broadcasters/juce_ChangeBroadcaster.cpp"
    "../../../JUCE/modules/juce_events/broadcasters/juce_ChangeBroadcaster.h"
    "../../../JUCE/modules/juce_events/broadcasters/juce_ChangeListener.h"
)
source_group("JUCE Modules\\juce_events\\broadcasters" FILES ${JUCE_Modules__juce_events__broadcasters})

set(JUCE_Modules__juce_events__interprocess
#    "../../../JUCE/modules/juce_events/interprocess/juce_ConnectedChildProcess.cpp"
    "../../../JUCE/modules/juce_events/interprocess/juce_ConnectedChildProcess.h"
#    "../../../JUCE/modules/juce_events/interprocess/juce_InterprocessConnection.cpp"
    "../../../JUCE/modules/juce_events/interprocess/juce_InterprocessConnection.h"
#    "../../../JUCE/modules/juce_events/interprocess/juce_InterprocessConnectionServer.cpp"
    "../../../JUCE/modules/juce_events/interprocess/juce_InterprocessConnectionServer.h"
#    "../../../JUCE/modules/juce_events/interprocess/juce_NetworkServiceDiscovery.cpp"
    "../../../JUCE/modules/juce_events/interprocess/juce_NetworkServiceDiscovery.h"
)
source_group("JUCE Modules\\juce_events\\interprocess" FILES ${JUCE_Modules__juce_events__interprocess})

set(JUCE_Modules__juce_events__messages
#    "../../../JUCE/modules/juce_events/messages/juce_ApplicationBase.cpp"
    "../../../JUCE/modules/juce_events/messages/juce_ApplicationBase.h"
    "../../../JUCE/modules/juce_events/messages/juce_CallbackMessage.h"
#    "../../../JUCE/modules/juce_events/messages/juce_DeletedAtShutdown.cpp"
    "../../../JUCE/modules/juce_events/messages/juce_DeletedAtShutdown.h"
    "../../../JUCE/modules/juce_events/messages/juce_Initialisation.h"
    "../../../JUCE/modules/juce_events/messages/juce_Message.h"
#    "../../../JUCE/modules/juce_events/messages/juce_MessageListener.cpp"
    "../../../JUCE/modules/juce_events/messages/juce_MessageListener.h"
#    "../../../JUCE/modules/juce_events/messages/juce_MessageManager.cpp"
    "../../../JUCE/modules/juce_events/messages/juce_MessageManager.h"
    "../../../JUCE/modules/juce_events/messages/juce_MountedVolumeListChangeDetector.h"
    "../../../JUCE/modules/juce_events/messages/juce_NotificationType.h"
)
source_group("JUCE Modules\\juce_events\\messages" FILES ${JUCE_Modules__juce_events__messages})

set(JUCE_Modules__juce_events__native
#    "../../../JUCE/modules/juce_events/native/juce_android_Messaging.cpp"
    "../../../JUCE/modules/juce_events/native/juce_linux_EventLoop.h"
    "../../../JUCE/modules/juce_events/native/juce_linux_EventLoopInternal.h"
#    "../../../JUCE/modules/juce_events/native/juce_linux_Messaging.cpp"
    "../../../JUCE/modules/juce_events/native/juce_osx_MessageQueue.h"
#    "../../../JUCE/modules/juce_events/native/juce_ScopedLowPowerModeDisabler.cpp"
    "../../../JUCE/modules/juce_events/native/juce_ScopedLowPowerModeDisabler.h"
    "../../../JUCE/modules/juce_events/native/juce_win32_HiddenMessageWindow.h"
#    "../../../JUCE/modules/juce_events/native/juce_win32_Messaging.cpp"
#    "../../../JUCE/modules/juce_events/native/juce_win32_WinRTWrapper.cpp"
    "../../../JUCE/modules/juce_events/native/juce_win32_WinRTWrapper.h"
)
source_group("JUCE Modules\\juce_events\\native" FILES ${JUCE_Modules__juce_events__native})

set(JUCE_Modules__juce_events__timers
#    "../../../JUCE/modules/juce_events/timers/juce_MultiTimer.cpp"
    "../../../JUCE/modules/juce_events/timers/juce_MultiTimer.h"
#    "../../../JUCE/modules/juce_events/timers/juce_Timer.cpp"
    "../../../JUCE/modules/juce_events/timers/juce_Timer.h"
)
source_group("JUCE Modules\\juce_events\\timers" FILES ${JUCE_Modules__juce_events__timers})

set(JUCE_Modules__juce_graphics
#    "../../../JUCE/modules/juce_graphics/juce_graphics.cpp"
    "../../../JUCE/modules/juce_graphics/juce_graphics.h"
)
source_group("JUCE Modules\\juce_graphics" FILES ${JUCE_Modules__juce_graphics})

set(JUCE_Modules__juce_graphics__colour
#    "../../../JUCE/modules/juce_graphics/colour/juce_Colour.cpp"
    "../../../JUCE/modules/juce_graphics/colour/juce_Colour.h"
#    "../../../JUCE/modules/juce_graphics/colour/juce_ColourGradient.cpp"
    "../../../JUCE/modules/juce_graphics/colour/juce_ColourGradient.h"
#    "../../../JUCE/modules/juce_graphics/colour/juce_Colours.cpp"
    "../../../JUCE/modules/juce_graphics/colour/juce_Colours.h"
#    "../../../JUCE/modules/juce_graphics/colour/juce_FillType.cpp"
    "../../../JUCE/modules/juce_graphics/colour/juce_FillType.h"
    "../../../JUCE/modules/juce_graphics/colour/juce_PixelFormats.h"
)
source_group("JUCE Modules\\juce_graphics\\colour" FILES ${JUCE_Modules__juce_graphics__colour})

set(JUCE_Modules__juce_graphics__contexts
#    "../../../JUCE/modules/juce_graphics/contexts/juce_GraphicsContext.cpp"
    "../../../JUCE/modules/juce_graphics/contexts/juce_GraphicsContext.h"
    "../../../JUCE/modules/juce_graphics/contexts/juce_LowLevelGraphicsContext.h"
#    "../../../JUCE/modules/juce_graphics/contexts/juce_LowLevelGraphicsPostScriptRenderer.cpp"
    "../../../JUCE/modules/juce_graphics/contexts/juce_LowLevelGraphicsPostScriptRenderer.h"
#    "../../../JUCE/modules/juce_graphics/contexts/juce_LowLevelGraphicsSoftwareRenderer.cpp"
    "../../../JUCE/modules/juce_graphics/contexts/juce_LowLevelGraphicsSoftwareRenderer.h"
)
source_group("JUCE Modules\\juce_graphics\\contexts" FILES ${JUCE_Modules__juce_graphics__contexts})

set(JUCE_Modules__juce_graphics__effects
#    "../../../JUCE/modules/juce_graphics/effects/juce_DropShadowEffect.cpp"
    "../../../JUCE/modules/juce_graphics/effects/juce_DropShadowEffect.h"
#    "../../../JUCE/modules/juce_graphics/effects/juce_GlowEffect.cpp"
    "../../../JUCE/modules/juce_graphics/effects/juce_GlowEffect.h"
    "../../../JUCE/modules/juce_graphics/effects/juce_ImageEffectFilter.h"
)
source_group("JUCE Modules\\juce_graphics\\effects" FILES ${JUCE_Modules__juce_graphics__effects})

set(JUCE_Modules__juce_graphics__fonts
#    "../../../JUCE/modules/juce_graphics/fonts/juce_AttributedString.cpp"
    "../../../JUCE/modules/juce_graphics/fonts/juce_AttributedString.h"
#    "../../../JUCE/modules/juce_graphics/fonts/juce_CustomTypeface.cpp"
    "../../../JUCE/modules/juce_graphics/fonts/juce_CustomTypeface.h"
#    "../../../JUCE/modules/juce_graphics/fonts/juce_Font.cpp"
    "../../../JUCE/modules/juce_graphics/fonts/juce_Font.h"
#    "../../../JUCE/modules/juce_graphics/fonts/juce_GlyphArrangement.cpp"
    "../../../JUCE/modules/juce_graphics/fonts/juce_GlyphArrangement.h"
#    "../../../JUCE/modules/juce_graphics/fonts/juce_TextLayout.cpp"
    "../../../JUCE/modules/juce_graphics/fonts/juce_TextLayout.h"
#    "../../../JUCE/modules/juce_graphics/fonts/juce_Typeface.cpp"
    "../../../JUCE/modules/juce_graphics/fonts/juce_Typeface.h"
)
source_group("JUCE Modules\\juce_graphics\\fonts" FILES ${JUCE_Modules__juce_graphics__fonts})

set(JUCE_Modules__juce_graphics__geometry
#    "../../../JUCE/modules/juce_graphics/geometry/juce_AffineTransform.cpp"
    "../../../JUCE/modules/juce_graphics/geometry/juce_AffineTransform.h"
    "../../../JUCE/modules/juce_graphics/geometry/juce_BorderSize.h"
#    "../../../JUCE/modules/juce_graphics/geometry/juce_EdgeTable.cpp"
    "../../../JUCE/modules/juce_graphics/geometry/juce_EdgeTable.h"
    "../../../JUCE/modules/juce_graphics/geometry/juce_Line.h"
    "../../../JUCE/modules/juce_graphics/geometry/juce_Parallelogram.h"
#    "../../../JUCE/modules/juce_graphics/geometry/juce_Path.cpp"
    "../../../JUCE/modules/juce_graphics/geometry/juce_Path.h"
#    "../../../JUCE/modules/juce_graphics/geometry/juce_PathIterator.cpp"
    "../../../JUCE/modules/juce_graphics/geometry/juce_PathIterator.h"
#    "../../../JUCE/modules/juce_graphics/geometry/juce_PathStrokeType.cpp"
    "../../../JUCE/modules/juce_graphics/geometry/juce_PathStrokeType.h"
    "../../../JUCE/modules/juce_graphics/geometry/juce_Point.h"
    "../../../JUCE/modules/juce_graphics/geometry/juce_Rectangle.h"
#    "../../../JUCE/modules/juce_graphics/geometry/juce_Rectangle_test.cpp"
    "../../../JUCE/modules/juce_graphics/geometry/juce_RectangleList.h"
)
source_group("JUCE Modules\\juce_graphics\\geometry" FILES ${JUCE_Modules__juce_graphics__geometry})

set(JUCE_Modules__juce_graphics__image_formats
#    "../../../JUCE/modules/juce_graphics/image_formats/juce_GIFLoader.cpp"
#    "../../../JUCE/modules/juce_graphics/image_formats/juce_JPEGLoader.cpp"
#    "../../../JUCE/modules/juce_graphics/image_formats/juce_PNGLoader.cpp"
)
source_group("JUCE Modules\\juce_graphics\\image_formats" FILES ${JUCE_Modules__juce_graphics__image_formats})

set(JUCE_Modules__juce_graphics__image_formats__jpglib
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/cderror.h"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/changes to libjpeg for JUCE.txt"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcapimin.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcapistd.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jccoefct.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jccolor.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcdctmgr.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jchuff.c"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jchuff.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcinit.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcmainct.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcmarker.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcmaster.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcomapi.c"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jconfig.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcparam.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcphuff.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcprepct.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jcsample.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jctrans.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdapimin.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdapistd.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdatasrc.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdcoefct.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdcolor.c"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdct.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jddctmgr.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdhuff.c"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdhuff.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdinput.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdmainct.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdmarker.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdmaster.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdmerge.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdphuff.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdpostct.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdsample.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jdtrans.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jerror.c"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jerror.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jfdctflt.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jfdctfst.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jfdctint.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jidctflt.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jidctfst.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jidctint.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jidctred.c"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jinclude.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jmemmgr.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jmemnobs.c"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jmemsys.h"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jmorecfg.h"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jpegint.h"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jpeglib.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jquant1.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jquant2.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jutils.c"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/jversion.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/transupp.c"
    "../../../JUCE/modules/juce_graphics/image_formats/jpglib/transupp.h"
)
source_group("JUCE Modules\\juce_graphics\\image_formats\\jpglib" FILES ${JUCE_Modules__juce_graphics__image_formats__jpglib})

set(JUCE_Modules__juce_graphics__image_formats__pnglib
    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/libpng_readme.txt"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/png.c"
    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/png.h"
    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngconf.h"
    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngdebug.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngerror.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngget.c"
    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pnginfo.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngmem.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngpread.c"
    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngpriv.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngread.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngrio.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngrtran.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngrutil.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngset.c"
    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngstruct.h"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngtrans.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngwio.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngwrite.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngwtran.c"
#    "../../../JUCE/modules/juce_graphics/image_formats/pnglib/pngwutil.c"
)
source_group("JUCE Modules\\juce_graphics\\image_formats\\pnglib" FILES ${JUCE_Modules__juce_graphics__image_formats__pnglib})

set(JUCE_Modules__juce_graphics__images
#    "../../../JUCE/modules/juce_graphics/images/juce_Image.cpp"
    "../../../JUCE/modules/juce_graphics/images/juce_Image.h"
#    "../../../JUCE/modules/juce_graphics/images/juce_ImageCache.cpp"
    "../../../JUCE/modules/juce_graphics/images/juce_ImageCache.h"
#    "../../../JUCE/modules/juce_graphics/images/juce_ImageConvolutionKernel.cpp"
    "../../../JUCE/modules/juce_graphics/images/juce_ImageConvolutionKernel.h"
#    "../../../JUCE/modules/juce_graphics/images/juce_ImageFileFormat.cpp"
    "../../../JUCE/modules/juce_graphics/images/juce_ImageFileFormat.h"
    "../../../JUCE/modules/juce_graphics/images/juce_ScaledImage.h"
)
source_group("JUCE Modules\\juce_graphics\\images" FILES ${JUCE_Modules__juce_graphics__images})

set(JUCE_Modules__juce_graphics__native
#    "../../../JUCE/modules/juce_graphics/native/juce_android_Fonts.cpp"
#    "../../../JUCE/modules/juce_graphics/native/juce_android_GraphicsContext.cpp"
#    "../../../JUCE/modules/juce_graphics/native/juce_android_IconHelpers.cpp"
#    "../../../JUCE/modules/juce_graphics/native/juce_freetype_Fonts.cpp"
#    "../../../JUCE/modules/juce_graphics/native/juce_linux_Fonts.cpp"
#    "../../../JUCE/modules/juce_graphics/native/juce_linux_IconHelpers.cpp"
    "../../../JUCE/modules/juce_graphics/native/juce_mac_CoreGraphicsContext.h"
    "../../../JUCE/modules/juce_graphics/native/juce_mac_CoreGraphicsHelpers.h"
#    "../../../JUCE/modules/juce_graphics/native/juce_mac_IconHelpers.cpp"
    "../../../JUCE/modules/juce_graphics/native/juce_RenderingHelpers.h"
#    "../../../JUCE/modules/juce_graphics/native/juce_win32_Direct2DGraphicsContext.cpp"
    "../../../JUCE/modules/juce_graphics/native/juce_win32_Direct2DGraphicsContext.h"
#    "../../../JUCE/modules/juce_graphics/native/juce_win32_DirectWriteTypeface.cpp"
#    "../../../JUCE/modules/juce_graphics/native/juce_win32_DirectWriteTypeLayout.cpp"
#    "../../../JUCE/modules/juce_graphics/native/juce_win32_Fonts.cpp"
#    "../../../JUCE/modules/juce_graphics/native/juce_win32_IconHelpers.cpp"
)
source_group("JUCE Modules\\juce_graphics\\native" FILES ${JUCE_Modules__juce_graphics__native})

set(JUCE_Modules__juce_graphics__placement
    "../../../JUCE/modules/juce_graphics/placement/juce_Justification.h"
#    "../../../JUCE/modules/juce_graphics/placement/juce_RectanglePlacement.cpp"
    "../../../JUCE/modules/juce_graphics/placement/juce_RectanglePlacement.h"
)
source_group("JUCE Modules\\juce_graphics\\placement" FILES ${JUCE_Modules__juce_graphics__placement})

set(JUCE_Modules__juce_gui_basics
#    "../../../JUCE/modules/juce_gui_basics/juce_gui_basics.cpp"
    "../../../JUCE/modules/juce_gui_basics/juce_gui_basics.h"
)
source_group("JUCE Modules\\juce_gui_basics" FILES ${JUCE_Modules__juce_gui_basics})

set(JUCE_Modules__juce_gui_basics__accessibility
#    "../../../JUCE/modules/juce_gui_basics/accessibility/juce_AccessibilityHandler.cpp"
    "../../../JUCE/modules/juce_gui_basics/accessibility/juce_AccessibilityHandler.h"
    "../../../JUCE/modules/juce_gui_basics/accessibility/juce_AccessibilityState.h"
)
source_group("JUCE Modules\\juce_gui_basics\\accessibility" FILES ${JUCE_Modules__juce_gui_basics__accessibility})

set(JUCE_Modules__juce_gui_basics__accessibility__enums
    "../../../JUCE/modules/juce_gui_basics/accessibility/enums/juce_AccessibilityActions.h"
    "../../../JUCE/modules/juce_gui_basics/accessibility/enums/juce_AccessibilityEvent.h"
    "../../../JUCE/modules/juce_gui_basics/accessibility/enums/juce_AccessibilityRole.h"
)
source_group("JUCE Modules\\juce_gui_basics\\accessibility\\enums" FILES ${JUCE_Modules__juce_gui_basics__accessibility__enums})

set(JUCE_Modules__juce_gui_basics__accessibility__interfaces
    "../../../JUCE/modules/juce_gui_basics/accessibility/interfaces/juce_AccessibilityCellInterface.h"
    "../../../JUCE/modules/juce_gui_basics/accessibility/interfaces/juce_AccessibilityTableInterface.h"
    "../../../JUCE/modules/juce_gui_basics/accessibility/interfaces/juce_AccessibilityTextInterface.h"
    "../../../JUCE/modules/juce_gui_basics/accessibility/interfaces/juce_AccessibilityValueInterface.h"
)
source_group("JUCE Modules\\juce_gui_basics\\accessibility\\interfaces" FILES ${JUCE_Modules__juce_gui_basics__accessibility__interfaces})

set(JUCE_Modules__juce_gui_basics__application
#    "../../../JUCE/modules/juce_gui_basics/application/juce_Application.cpp"
    "../../../JUCE/modules/juce_gui_basics/application/juce_Application.h"
)
source_group("JUCE Modules\\juce_gui_basics\\application" FILES ${JUCE_Modules__juce_gui_basics__application})

set(JUCE_Modules__juce_gui_basics__buttons
#    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ArrowButton.cpp"
    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ArrowButton.h"
#    "../../../JUCE/modules/juce_gui_basics/buttons/juce_Button.cpp"
    "../../../JUCE/modules/juce_gui_basics/buttons/juce_Button.h"
#    "../../../JUCE/modules/juce_gui_basics/buttons/juce_DrawableButton.cpp"
    "../../../JUCE/modules/juce_gui_basics/buttons/juce_DrawableButton.h"
#    "../../../JUCE/modules/juce_gui_basics/buttons/juce_HyperlinkButton.cpp"
    "../../../JUCE/modules/juce_gui_basics/buttons/juce_HyperlinkButton.h"
#    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ImageButton.cpp"
    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ImageButton.h"
#    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ShapeButton.cpp"
    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ShapeButton.h"
#    "../../../JUCE/modules/juce_gui_basics/buttons/juce_TextButton.cpp"
    "../../../JUCE/modules/juce_gui_basics/buttons/juce_TextButton.h"
#    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ToggleButton.cpp"
    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ToggleButton.h"
#    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ToolbarButton.cpp"
    "../../../JUCE/modules/juce_gui_basics/buttons/juce_ToolbarButton.h"
)
source_group("JUCE Modules\\juce_gui_basics\\buttons" FILES ${JUCE_Modules__juce_gui_basics__buttons})

set(JUCE_Modules__juce_gui_basics__commands
    "../../../JUCE/modules/juce_gui_basics/commands/juce_ApplicationCommandID.h"
#    "../../../JUCE/modules/juce_gui_basics/commands/juce_ApplicationCommandInfo.cpp"
    "../../../JUCE/modules/juce_gui_basics/commands/juce_ApplicationCommandInfo.h"
#    "../../../JUCE/modules/juce_gui_basics/commands/juce_ApplicationCommandManager.cpp"
    "../../../JUCE/modules/juce_gui_basics/commands/juce_ApplicationCommandManager.h"
#    "../../../JUCE/modules/juce_gui_basics/commands/juce_ApplicationCommandTarget.cpp"
    "../../../JUCE/modules/juce_gui_basics/commands/juce_ApplicationCommandTarget.h"
#    "../../../JUCE/modules/juce_gui_basics/commands/juce_KeyPressMappingSet.cpp"
    "../../../JUCE/modules/juce_gui_basics/commands/juce_KeyPressMappingSet.h"
)
source_group("JUCE Modules\\juce_gui_basics\\commands" FILES ${JUCE_Modules__juce_gui_basics__commands})

set(JUCE_Modules__juce_gui_basics__components
    "../../../JUCE/modules/juce_gui_basics/components/juce_CachedComponentImage.h"
#    "../../../JUCE/modules/juce_gui_basics/components/juce_Component.cpp"
    "../../../JUCE/modules/juce_gui_basics/components/juce_Component.h"
#    "../../../JUCE/modules/juce_gui_basics/components/juce_ComponentListener.cpp"
    "../../../JUCE/modules/juce_gui_basics/components/juce_ComponentListener.h"
    "../../../JUCE/modules/juce_gui_basics/components/juce_ComponentTraverser.h"
#    "../../../JUCE/modules/juce_gui_basics/components/juce_FocusTraverser.cpp"
    "../../../JUCE/modules/juce_gui_basics/components/juce_FocusTraverser.h"
#    "../../../JUCE/modules/juce_gui_basics/components/juce_ModalComponentManager.cpp"
    "../../../JUCE/modules/juce_gui_basics/components/juce_ModalComponentManager.h"
)
source_group("JUCE Modules\\juce_gui_basics\\components" FILES ${JUCE_Modules__juce_gui_basics__components})

set(JUCE_Modules__juce_gui_basics__desktop
#    "../../../JUCE/modules/juce_gui_basics/desktop/juce_Desktop.cpp"
    "../../../JUCE/modules/juce_gui_basics/desktop/juce_Desktop.h"
#    "../../../JUCE/modules/juce_gui_basics/desktop/juce_Displays.cpp"
    "../../../JUCE/modules/juce_gui_basics/desktop/juce_Displays.h"
)
source_group("JUCE Modules\\juce_gui_basics\\desktop" FILES ${JUCE_Modules__juce_gui_basics__desktop})

set(JUCE_Modules__juce_gui_basics__drawables
#    "../../../JUCE/modules/juce_gui_basics/drawables/juce_Drawable.cpp"
    "../../../JUCE/modules/juce_gui_basics/drawables/juce_Drawable.h"
#    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableComposite.cpp"
    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableComposite.h"
#    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableImage.cpp"
    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableImage.h"
#    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawablePath.cpp"
    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawablePath.h"
#    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableRectangle.cpp"
    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableRectangle.h"
#    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableShape.cpp"
    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableShape.h"
#    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableText.cpp"
    "../../../JUCE/modules/juce_gui_basics/drawables/juce_DrawableText.h"
#    "../../../JUCE/modules/juce_gui_basics/drawables/juce_SVGParser.cpp"
)
source_group("JUCE Modules\\juce_gui_basics\\drawables" FILES ${JUCE_Modules__juce_gui_basics__drawables})

set(JUCE_Modules__juce_gui_basics__filebrowser
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_ContentSharer.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_ContentSharer.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_DirectoryContentsDisplayComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_DirectoryContentsDisplayComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_DirectoryContentsList.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_DirectoryContentsList.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileBrowserComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileBrowserComponent.h"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileBrowserListener.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileChooser.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileChooser.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileChooserDialogBox.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileChooserDialogBox.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileListComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileListComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FilenameComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FilenameComponent.h"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FilePreviewComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileSearchPathListComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileSearchPathListComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileTreeComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_FileTreeComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_ImagePreviewComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/filebrowser/juce_ImagePreviewComponent.h"
)
source_group("JUCE Modules\\juce_gui_basics\\filebrowser" FILES ${JUCE_Modules__juce_gui_basics__filebrowser})

set(JUCE_Modules__juce_gui_basics__keyboard
#    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_CaretComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_CaretComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_KeyboardFocusTraverser.cpp"
    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_KeyboardFocusTraverser.h"
#    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_KeyListener.cpp"
    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_KeyListener.h"
#    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_KeyPress.cpp"
    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_KeyPress.h"
#    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_ModifierKeys.cpp"
    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_ModifierKeys.h"
    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_SystemClipboard.h"
    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_TextEditorKeyMapper.h"
    "../../../JUCE/modules/juce_gui_basics/keyboard/juce_TextInputTarget.h"
)
source_group("JUCE Modules\\juce_gui_basics\\keyboard" FILES ${JUCE_Modules__juce_gui_basics__keyboard})

set(JUCE_Modules__juce_gui_basics__layout
    "../../../JUCE/modules/juce_gui_basics/layout/juce_AnimatedPosition.h"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_AnimatedPositionBehaviours.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_ComponentAnimator.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_ComponentAnimator.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_ComponentBoundsConstrainer.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_ComponentBoundsConstrainer.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_ComponentBuilder.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_ComponentBuilder.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_ComponentMovementWatcher.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_ComponentMovementWatcher.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_ConcertinaPanel.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_ConcertinaPanel.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_FlexBox.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_FlexBox.h"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_FlexItem.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_Grid.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_Grid.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_GridItem.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_GridItem.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_GroupComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_GroupComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_MultiDocumentPanel.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_MultiDocumentPanel.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_ResizableBorderComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_ResizableBorderComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_ResizableCornerComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_ResizableCornerComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_ResizableEdgeComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_ResizableEdgeComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_ScrollBar.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_ScrollBar.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_SidePanel.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_SidePanel.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_StretchableLayoutManager.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_StretchableLayoutManager.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_StretchableLayoutResizerBar.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_StretchableLayoutResizerBar.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_StretchableObjectResizer.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_StretchableObjectResizer.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_TabbedButtonBar.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_TabbedButtonBar.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_TabbedComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_TabbedComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/layout/juce_Viewport.cpp"
    "../../../JUCE/modules/juce_gui_basics/layout/juce_Viewport.h"
)
source_group("JUCE Modules\\juce_gui_basics\\layout" FILES ${JUCE_Modules__juce_gui_basics__layout})

set(JUCE_Modules__juce_gui_basics__lookandfeel
#    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel.cpp"
    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel.h"
#    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V1.cpp"
    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V1.h"
#    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V2.cpp"
    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V2.h"
#    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V3.cpp"
    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V3.h"
#    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V4.cpp"
    "../../../JUCE/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V4.h"
)
source_group("JUCE Modules\\juce_gui_basics\\lookandfeel" FILES ${JUCE_Modules__juce_gui_basics__lookandfeel})

set(JUCE_Modules__juce_gui_basics__menus
#    "../../../JUCE/modules/juce_gui_basics/menus/juce_BurgerMenuComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/menus/juce_BurgerMenuComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/menus/juce_MenuBarComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/menus/juce_MenuBarComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/menus/juce_MenuBarModel.cpp"
    "../../../JUCE/modules/juce_gui_basics/menus/juce_MenuBarModel.h"
#    "../../../JUCE/modules/juce_gui_basics/menus/juce_PopupMenu.cpp"
    "../../../JUCE/modules/juce_gui_basics/menus/juce_PopupMenu.h"
)
source_group("JUCE Modules\\juce_gui_basics\\menus" FILES ${JUCE_Modules__juce_gui_basics__menus})

set(JUCE_Modules__juce_gui_basics__misc
#    "../../../JUCE/modules/juce_gui_basics/misc/juce_BubbleComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/misc/juce_BubbleComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/misc/juce_DropShadower.cpp"
    "../../../JUCE/modules/juce_gui_basics/misc/juce_DropShadower.h"
#    "../../../JUCE/modules/juce_gui_basics/misc/juce_FocusOutline.cpp"
    "../../../JUCE/modules/juce_gui_basics/misc/juce_FocusOutline.h"
#    "../../../JUCE/modules/juce_gui_basics/misc/juce_JUCESplashScreen.cpp"
    "../../../JUCE/modules/juce_gui_basics/misc/juce_JUCESplashScreen.h"
)
source_group("JUCE Modules\\juce_gui_basics\\misc" FILES ${JUCE_Modules__juce_gui_basics__misc})

set(JUCE_Modules__juce_gui_basics__mouse
#    "../../../JUCE/modules/juce_gui_basics/mouse/juce_ComponentDragger.cpp"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_ComponentDragger.h"
#    "../../../JUCE/modules/juce_gui_basics/mouse/juce_DragAndDropContainer.cpp"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_DragAndDropContainer.h"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_DragAndDropTarget.h"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_FileDragAndDropTarget.h"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_LassoComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseCursor.cpp"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseCursor.h"
#    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseEvent.cpp"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseEvent.h"
#    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseInactivityDetector.cpp"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseInactivityDetector.h"
#    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseInputSource.cpp"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseInputSource.h"
#    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseListener.cpp"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_MouseListener.h"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_PointerState.h"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_SelectedItemSet.h"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_TextDragAndDropTarget.h"
    "../../../JUCE/modules/juce_gui_basics/mouse/juce_TooltipClient.h"
)
source_group("JUCE Modules\\juce_gui_basics\\mouse" FILES ${JUCE_Modules__juce_gui_basics__mouse})

set(JUCE_Modules__juce_gui_basics__native
#    "../../../JUCE/modules/juce_gui_basics/native/juce_android_ContentSharer.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/juce_android_FileChooser.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/juce_android_Windowing.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/juce_ios_ContentSharer.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/juce_linux_FileChooser.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/juce_linux_Windowing.cpp"
    "../../../JUCE/modules/juce_gui_basics/native/juce_mac_CGMetalLayerRenderer.h"
    "../../../JUCE/modules/juce_gui_basics/native/juce_mac_PerScreenDisplayLinks.h"
    "../../../JUCE/modules/juce_gui_basics/native/juce_MultiTouchMapper.h"
    "../../../JUCE/modules/juce_gui_basics/native/juce_ScopedDPIAwarenessDisabler.h"
#    "../../../JUCE/modules/juce_gui_basics/native/juce_win32_DragAndDrop.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/juce_win32_FileChooser.cpp"
    "../../../JUCE/modules/juce_gui_basics/native/juce_win32_ScopedThreadDPIAwarenessSetter.h"
#    "../../../JUCE/modules/juce_gui_basics/native/juce_win32_Windowing.cpp"
)
source_group("JUCE Modules\\juce_gui_basics\\native" FILES ${JUCE_Modules__juce_gui_basics__native})

set(JUCE_Modules__juce_gui_basics__native__accessibility
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_AccessibilityTextHelpers.h"
#    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_AccessibilityTextHelpers_test.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_android_Accessibility.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_Accessibility.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_AccessibilityElement.cpp"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_AccessibilityElement.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_ComInterfaces.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAExpandCollapseProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAGridItemProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAGridProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAHelpers.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAInvokeProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAProviderBase.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAProviders.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIARangeValueProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIASelectionProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIATextProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAToggleProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIATransformProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAValueProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_UIAWindowProvider.h"
    "../../../JUCE/modules/juce_gui_basics/native/accessibility/juce_win32_WindowsUIAWrapper.h"
)
source_group("JUCE Modules\\juce_gui_basics\\native\\accessibility" FILES ${JUCE_Modules__juce_gui_basics__native__accessibility})

set(JUCE_Modules__juce_gui_basics__native__x11
    "../../../JUCE/modules/juce_gui_basics/native/x11/juce_linux_ScopedWindowAssociation.h"
#    "../../../JUCE/modules/juce_gui_basics/native/x11/juce_linux_X11_DragAndDrop.cpp"
#    "../../../JUCE/modules/juce_gui_basics/native/x11/juce_linux_X11_Symbols.cpp"
    "../../../JUCE/modules/juce_gui_basics/native/x11/juce_linux_X11_Symbols.h"
#    "../../../JUCE/modules/juce_gui_basics/native/x11/juce_linux_XWindowSystem.cpp"
    "../../../JUCE/modules/juce_gui_basics/native/x11/juce_linux_XWindowSystem.h"
)
source_group("JUCE Modules\\juce_gui_basics\\native\\x11" FILES ${JUCE_Modules__juce_gui_basics__native__x11})

set(JUCE_Modules__juce_gui_basics__positioning
#    "../../../JUCE/modules/juce_gui_basics/positioning/juce_MarkerList.cpp"
    "../../../JUCE/modules/juce_gui_basics/positioning/juce_MarkerList.h"
#    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativeCoordinate.cpp"
    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativeCoordinate.h"
#    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativeCoordinatePositioner.cpp"
    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativeCoordinatePositioner.h"
#    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativeParallelogram.cpp"
    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativeParallelogram.h"
#    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativePoint.cpp"
    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativePoint.h"
#    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativePointPath.cpp"
    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativePointPath.h"
#    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativeRectangle.cpp"
    "../../../JUCE/modules/juce_gui_basics/positioning/juce_RelativeRectangle.h"
)
source_group("JUCE Modules\\juce_gui_basics\\positioning" FILES ${JUCE_Modules__juce_gui_basics__positioning})

set(JUCE_Modules__juce_gui_basics__properties
#    "../../../JUCE/modules/juce_gui_basics/properties/juce_BooleanPropertyComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/properties/juce_BooleanPropertyComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/properties/juce_ButtonPropertyComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/properties/juce_ButtonPropertyComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/properties/juce_ChoicePropertyComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/properties/juce_ChoicePropertyComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/properties/juce_MultiChoicePropertyComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/properties/juce_MultiChoicePropertyComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/properties/juce_PropertyComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/properties/juce_PropertyComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/properties/juce_PropertyPanel.cpp"
    "../../../JUCE/modules/juce_gui_basics/properties/juce_PropertyPanel.h"
#    "../../../JUCE/modules/juce_gui_basics/properties/juce_SliderPropertyComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/properties/juce_SliderPropertyComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/properties/juce_TextPropertyComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/properties/juce_TextPropertyComponent.h"
)
source_group("JUCE Modules\\juce_gui_basics\\properties" FILES ${JUCE_Modules__juce_gui_basics__properties})

set(JUCE_Modules__juce_gui_basics__widgets
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ComboBox.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ComboBox.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ImageComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ImageComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_Label.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_Label.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ListBox.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ListBox.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ProgressBar.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ProgressBar.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_Slider.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_Slider.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_TableHeaderComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_TableHeaderComponent.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_TableListBox.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_TableListBox.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_TextEditor.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_TextEditor.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_Toolbar.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_Toolbar.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ToolbarItemComponent.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ToolbarItemComponent.h"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ToolbarItemFactory.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ToolbarItemPalette.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_ToolbarItemPalette.h"
#    "../../../JUCE/modules/juce_gui_basics/widgets/juce_TreeView.cpp"
    "../../../JUCE/modules/juce_gui_basics/widgets/juce_TreeView.h"
)
source_group("JUCE Modules\\juce_gui_basics\\widgets" FILES ${JUCE_Modules__juce_gui_basics__widgets})

set(JUCE_Modules__juce_gui_basics__windows
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_AlertWindow.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_AlertWindow.h"
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_CallOutBox.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_CallOutBox.h"
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_ComponentPeer.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_ComponentPeer.h"
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_DialogWindow.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_DialogWindow.h"
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_DocumentWindow.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_DocumentWindow.h"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_MessageBoxOptions.h"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_NativeMessageBox.h"
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_ResizableWindow.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_ResizableWindow.h"
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_ThreadWithProgressWindow.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_ThreadWithProgressWindow.h"
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_TooltipWindow.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_TooltipWindow.h"
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_TopLevelWindow.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_TopLevelWindow.h"
#    "../../../JUCE/modules/juce_gui_basics/windows/juce_VBlankAttachement.cpp"
    "../../../JUCE/modules/juce_gui_basics/windows/juce_VBlankAttachement.h"
)
source_group("JUCE Modules\\juce_gui_basics\\windows" FILES ${JUCE_Modules__juce_gui_basics__windows})

set(JUCE_Modules__juce_gui_extra
#    "../../../JUCE/modules/juce_gui_extra/juce_gui_extra.cpp"
    "../../../JUCE/modules/juce_gui_extra/juce_gui_extra.h"
)
source_group("JUCE Modules\\juce_gui_extra" FILES ${JUCE_Modules__juce_gui_extra})

set(JUCE_Modules__juce_gui_extra__code_editor
#    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_CodeDocument.cpp"
    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_CodeDocument.h"
#    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_CodeEditorComponent.cpp"
    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_CodeEditorComponent.h"
    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_CodeTokeniser.h"
#    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_CPlusPlusCodeTokeniser.cpp"
    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_CPlusPlusCodeTokeniser.h"
    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_CPlusPlusCodeTokeniserFunctions.h"
#    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_LuaCodeTokeniser.cpp"
    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_LuaCodeTokeniser.h"
#    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_XMLCodeTokeniser.cpp"
    "../../../JUCE/modules/juce_gui_extra/code_editor/juce_XMLCodeTokeniser.h"
)
source_group("JUCE Modules\\juce_gui_extra\\code_editor" FILES ${JUCE_Modules__juce_gui_extra__code_editor})

set(JUCE_Modules__juce_gui_extra__documents
#    "../../../JUCE/modules/juce_gui_extra/documents/juce_FileBasedDocument.cpp"
    "../../../JUCE/modules/juce_gui_extra/documents/juce_FileBasedDocument.h"
)
source_group("JUCE Modules\\juce_gui_extra\\documents" FILES ${JUCE_Modules__juce_gui_extra__documents})

set(JUCE_Modules__juce_gui_extra__embedding
    "../../../JUCE/modules/juce_gui_extra/embedding/juce_ActiveXControlComponent.h"
    "../../../JUCE/modules/juce_gui_extra/embedding/juce_AndroidViewComponent.h"
    "../../../JUCE/modules/juce_gui_extra/embedding/juce_HWNDComponent.h"
    "../../../JUCE/modules/juce_gui_extra/embedding/juce_NSViewComponent.h"
    "../../../JUCE/modules/juce_gui_extra/embedding/juce_UIViewComponent.h"
    "../../../JUCE/modules/juce_gui_extra/embedding/juce_XEmbedComponent.h"
)
source_group("JUCE Modules\\juce_gui_extra\\embedding" FILES ${JUCE_Modules__juce_gui_extra__embedding})

set(JUCE_Modules__juce_gui_extra__misc
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_AnimatedAppComponent.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_AnimatedAppComponent.h"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_AppleRemote.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_BubbleMessageComponent.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_BubbleMessageComponent.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_ColourSelector.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_ColourSelector.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_KeyMappingEditorComponent.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_KeyMappingEditorComponent.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_LiveConstantEditor.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_LiveConstantEditor.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_PreferencesPanel.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_PreferencesPanel.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_PushNotifications.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_PushNotifications.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_RecentlyOpenedFilesList.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_RecentlyOpenedFilesList.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_SplashScreen.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_SplashScreen.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_SystemTrayIconComponent.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_SystemTrayIconComponent.h"
#    "../../../JUCE/modules/juce_gui_extra/misc/juce_WebBrowserComponent.cpp"
    "../../../JUCE/modules/juce_gui_extra/misc/juce_WebBrowserComponent.h"
)
source_group("JUCE Modules\\juce_gui_extra\\misc" FILES ${JUCE_Modules__juce_gui_extra__misc})

set(JUCE_Modules__juce_gui_extra__native
#    "../../../JUCE/modules/juce_gui_extra/native/juce_android_PushNotifications.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_android_WebBrowserComponent.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_AndroidViewComponent.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_ios_PushNotifications.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_linux_X11_SystemTrayIcon.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_linux_X11_WebBrowserComponent.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_linux_XEmbedComponent.cpp"
    "../../../JUCE/modules/juce_gui_extra/native/juce_mac_NSViewFrameWatcher.h"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_mac_PushNotifications.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_mac_SystemTrayIcon.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_win32_ActiveXComponent.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_win32_HWNDComponent.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_win32_SystemTrayIcon.cpp"
#    "../../../JUCE/modules/juce_gui_extra/native/juce_win32_WebBrowserComponent.cpp"
)
source_group("JUCE Modules\\juce_gui_extra\\native" FILES ${JUCE_Modules__juce_gui_extra__native})

set(Synth__Source
    "../../Source/Params.h"
    "../../Source/PluginEditor.cpp"
    "../../Source/PluginEditor.h"
    "../../Source/PluginProcessor.cpp"
    "../../Source/PluginProcessor.h"
)
source_group("Synth\\Source" FILES ${Synth__Source})

set(Synth__Source__dsp
    "../../Source/SpectrumAnalyzer.cpp"
    "../../Source/SpectrumAnalyzer.h"
    "../../Source/SynthSound.cpp"
    "../../Source/SynthSound.h"
    "../../Source/SynthVoice.cpp"
    "../../Source/SynthVoice.h"
    "../../Source/Waves.h"
)
source_group("Synth\\Source\\dsp" FILES ${Synth__Source__dsp})

set(Synth__Source__maximilian
    "../../../maximilian/maximilian.cpp"
    "../../../maximilian/maximilian.h"
)
source_group("Synth\\Source\\maximilian" FILES ${Synth__Source__maximilian})

set(Synth__Source__maximilian__libs
    "../../../maximilian/libs/fft.cpp"
    "../../../maximilian/libs/fft.h"
    "../../../maximilian/libs/maxiBark.cpp"
    "../../../maximilian/libs/maxiBark.h"
    "../../../maximilian/libs/maxiClock.cpp"
    "../../../maximilian/libs/maxiClock.h"
    "../../../maximilian/libs/maxiConvolve.cpp"
    "../../../maximilian/libs/maxiConvolve.h"
    "../../../maximilian/libs/maxiFFT.cpp"
    "../../../maximilian/libs/maxiFFT.h"
    "../../../maximilian/libs/maxiGrains.cpp"
    "../../../maximilian/libs/maxiGrains.h"
    "../../../maximilian/libs/maxim.h"
    "../../../maximilian/libs/maxiMFCC.cpp"
    "../../../maximilian/libs/maxiMFCC.h"
    "../../../maximilian/libs/maxiPolyBLEP.h"
    "../../../maximilian/libs/maxiReverb.cpp"
    "../../../maximilian/libs/maxiReverb.h"
    "../../../maximilian/libs/maxiSynths.cpp"
    "../../../maximilian/libs/maxiSynths.h"
    "../../../maximilian/libs/sineTable.h"
    "../../../maximilian/libs/stb_vorbis.c"
    "../../../maximilian/libs/stb_vorbis.h"
)
source_group("Synth\\Source\\maximilian\\libs" FILES ${Synth__Source__maximilian__libs})

set(Synth__Source__maximilian__libs__PolyBLEP
    "../../../maximilian/libs/PolyBLEP/PolyBLEP.cpp"
    "../../../maximilian/libs/PolyBLEP/PolyBLEP.h"
    "../../../maximilian/libs/PolyBLEP/README.md"
)
source_group("Synth\\Source\\maximilian\\libs\\PolyBLEP" FILES ${Synth__Source__maximilian__libs__PolyBLEP})

set(Synth__Source__ui
    "../../Source/AdsrPanel.cpp"
    "../../Source/AdsrPanel.h"
    "../../Source/EnvelopeComponent.cpp"
    "../../Source/EnvelopeComponent.h"
    "../../Source/FilterComponent.cpp"
    "../../Source/FilterComponent.h"
    "../../Source/OscControlJPanel.cpp"
    "../../Source/OscControlJPanel.h"
    "../../Source/Oscillator.cpp"
    "../../Source/Oscillator.h"
    "../../Source/OscWaveTable.cpp"
    "../../Source/OscWaveTable.h"
)
source_group("Synth\\Source\\ui" FILES ${Synth__Source__ui})

set(ALL_FILES
    ${JUCE_Library_Code}
    ${JUCE_Modules__juce_audio_basics}
    ${JUCE_Modules__juce_audio_basics__audio_play_head}
    ${JUCE_Modules__juce_audio_basics__buffers}
    ${JUCE_Modules__juce_audio_basics__midi}
    ${JUCE_Modules__juce_audio_basics__midi__ump}
    ${JUCE_Modules__juce_audio_basics__mpe}
    ${JUCE_Modules__juce_audio_basics__native}
    ${JUCE_Modules__juce_audio_basics__sources}
    ${JUCE_Modules__juce_audio_basics__synthesisers}
    ${JUCE_Modules__juce_audio_basics__utilities}
    ${JUCE_Modules__juce_audio_devices}
    ${JUCE_Modules__juce_audio_devices__audio_io}
    ${JUCE_Modules__juce_audio_devices__midi_io}
    ${JUCE_Modules__juce_audio_devices__midi_io__ump}
    ${JUCE_Modules__juce_audio_devices__native}
    ${JUCE_Modules__juce_audio_devices__native__oboe}
    ${JUCE_Modules__juce_audio_devices__native__oboe__include__oboe}
    ${JUCE_Modules__juce_audio_devices__native__oboe__src__aaudio}
    ${JUCE_Modules__juce_audio_devices__native__oboe__src__common}
    ${JUCE_Modules__juce_audio_devices__native__oboe__src__fifo}
    ${JUCE_Modules__juce_audio_devices__native__oboe__src__flowgraph}
    ${JUCE_Modules__juce_audio_devices__native__oboe__src__flowgraph__resampler}
    ${JUCE_Modules__juce_audio_devices__native__oboe__src__opensles}
    ${JUCE_Modules__juce_audio_devices__sources}
    ${JUCE_Modules__juce_audio_formats}
    ${JUCE_Modules__juce_audio_formats__codecs}
    ${JUCE_Modules__juce_audio_formats__codecs__flac}
    ${JUCE_Modules__juce_audio_formats__codecs__flac__libFLAC}
    ${JUCE_Modules__juce_audio_formats__codecs__flac__libFLAC__include__private}
    ${JUCE_Modules__juce_audio_formats__codecs__flac__libFLAC__include__protected}
    ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis}
    ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7}
    ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib}
    ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__books__coupled}
    ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__books__floor}
    ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__books__uncoupled}
    ${JUCE_Modules__juce_audio_formats__codecs__oggvorbis__libvorbis-1.3.7__lib__modes}
    ${JUCE_Modules__juce_audio_formats__format}
    ${JUCE_Modules__juce_audio_formats__sampler}
    ${JUCE_Modules__juce_audio_plugin_client}
    ${JUCE_Modules__juce_audio_plugin_client__ARA}
    ${JUCE_Modules__juce_audio_plugin_client__utility}
    ${JUCE_Modules__juce_audio_processors}
    ${JUCE_Modules__juce_audio_processors__format}
    ${JUCE_Modules__juce_audio_processors__format_types}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lilv__lilv}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lilv__src}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lilv__src__zix}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__atom}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__buf-size}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__core}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__data-access}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__dynmanifest}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__event}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__instance-access}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__log}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__midi}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__morph}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__options}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__parameters}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__patch}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__port-groups}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__port-props}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__presets}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__resize-port}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__state}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__time}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__ui}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__units}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__uri-map}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__urid}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__lv2__lv2__worker}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__serd__serd}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__serd__src}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sord__sord}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sord__src}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sord__src__zix}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sratom__sratom}
    ${JUCE_Modules__juce_audio_processors__format_types__LV2_SDK__sratom__src}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base__source}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base__thread__include}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__base__thread__source}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces__base}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces__gui}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__pluginterfaces__vst}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk__source__common}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk__source__vst}
    ${JUCE_Modules__juce_audio_processors__format_types__VST3_SDK__public.sdk__source__vst__hosting}
    ${JUCE_Modules__juce_audio_processors__format_types__pslextensions}
    ${JUCE_Modules__juce_audio_processors__processors}
    ${JUCE_Modules__juce_audio_processors__scanning}
    ${JUCE_Modules__juce_audio_processors__utilities}
    ${JUCE_Modules__juce_audio_processors__utilities__ARA}
    ${JUCE_Modules__juce_audio_utils}
    ${JUCE_Modules__juce_audio_utils__audio_cd}
    ${JUCE_Modules__juce_audio_utils__gui}
    ${JUCE_Modules__juce_audio_utils__native}
    ${JUCE_Modules__juce_audio_utils__players}
    ${JUCE_Modules__juce_core}
    ${JUCE_Modules__juce_core__containers}
    ${JUCE_Modules__juce_core__files}
    ${JUCE_Modules__juce_core__javascript}
    ${JUCE_Modules__juce_core__logging}
    ${JUCE_Modules__juce_core__maths}
    ${JUCE_Modules__juce_core__memory}
    ${JUCE_Modules__juce_core__misc}
    ${JUCE_Modules__juce_core__native}
    ${JUCE_Modules__juce_core__native__java}
    ${JUCE_Modules__juce_core__network}
    ${JUCE_Modules__juce_core__streams}
    ${JUCE_Modules__juce_core__system}
    ${JUCE_Modules__juce_core__text}
    ${JUCE_Modules__juce_core__threads}
    ${JUCE_Modules__juce_core__time}
    ${JUCE_Modules__juce_core__unit_tests}
    ${JUCE_Modules__juce_core__xml}
    ${JUCE_Modules__juce_core__zip}
    ${JUCE_Modules__juce_core__zip__zlib}
    ${JUCE_Modules__juce_data_structures}
    ${JUCE_Modules__juce_data_structures__app_properties}
    ${JUCE_Modules__juce_data_structures__undomanager}
    ${JUCE_Modules__juce_data_structures__values}
    ${JUCE_Modules__juce_dsp}
    ${JUCE_Modules__juce_dsp__containers}
    ${JUCE_Modules__juce_dsp__filter_design}
    ${JUCE_Modules__juce_dsp__frequency}
    ${JUCE_Modules__juce_dsp__maths}
    ${JUCE_Modules__juce_dsp__native}
    ${JUCE_Modules__juce_dsp__processors}
    ${JUCE_Modules__juce_dsp__widgets}
    ${JUCE_Modules__juce_events}
    ${JUCE_Modules__juce_events__broadcasters}
    ${JUCE_Modules__juce_events__interprocess}
    ${JUCE_Modules__juce_events__messages}
    ${JUCE_Modules__juce_events__native}
    ${JUCE_Modules__juce_events__timers}
    ${JUCE_Modules__juce_graphics}
    ${JUCE_Modules__juce_graphics__colour}
    ${JUCE_Modules__juce_graphics__contexts}
    ${JUCE_Modules__juce_graphics__effects}
    ${JUCE_Modules__juce_graphics__fonts}
    ${JUCE_Modules__juce_graphics__geometry}
    ${JUCE_Modules__juce_graphics__image_formats}
    ${JUCE_Modules__juce_graphics__image_formats__jpglib}
    ${JUCE_Modules__juce_graphics__image_formats__pnglib}
    ${JUCE_Modules__juce_graphics__images}
    ${JUCE_Modules__juce_graphics__native}
    ${JUCE_Modules__juce_graphics__placement}
    ${JUCE_Modules__juce_gui_basics}
    ${JUCE_Modules__juce_gui_basics__accessibility}
    ${JUCE_Modules__juce_gui_basics__accessibility__enums}
    ${JUCE_Modules__juce_gui_basics__accessibility__interfaces}
    ${JUCE_Modules__juce_gui_basics__application}
    ${JUCE_Modules__juce_gui_basics__buttons}
    ${JUCE_Modules__juce_gui_basics__commands}
    ${JUCE_Modules__juce_gui_basics__components}
    ${JUCE_Modules__juce_gui_basics__desktop}
    ${JUCE_Modules__juce_gui_basics__drawables}
    ${JUCE_Modules__juce_gui_basics__filebrowser}
    ${JUCE_Modules__juce_gui_basics__keyboard}
    ${JUCE_Modules__juce_gui_basics__layout}
    ${JUCE_Modules__juce_gui_basics__lookandfeel}
    ${JUCE_Modules__juce_gui_basics__menus}
    ${JUCE_Modules__juce_gui_basics__misc}
    ${JUCE_Modules__juce_gui_basics__mouse}
    ${JUCE_Modules__juce_gui_basics__native}
    ${JUCE_Modules__juce_gui_basics__native__accessibility}
    ${JUCE_Modules__juce_gui_basics__native__x11}
    ${JUCE_Modules__juce_gui_basics__positioning}
    ${JUCE_Modules__juce_gui_basics__properties}
    ${JUCE_Modules__juce_gui_basics__widgets}
    ${JUCE_Modules__juce_gui_basics__windows}
    ${JUCE_Modules__juce_gui_extra}
    ${JUCE_Modules__juce_gui_extra__code_editor}
    ${JUCE_Modules__juce_gui_extra__documents}
    ${JUCE_Modules__juce_gui_extra__embedding}
    ${JUCE_Modules__juce_gui_extra__misc}
    ${JUCE_Modules__juce_gui_extra__native}
    ${Synth__Source}
    ${Synth__Source__dsp}
    ${Synth__Source__maximilian}
    ${Synth__Source__maximilian__libs}
    ${Synth__Source__maximilian__libs__PolyBLEP}
    ${Synth__Source__ui}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "Synth"
    TARGET_NAME_RELEASE "Synth"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/Shared Code"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/Shared Code"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    PDB_OUTPUT_DIRECTORY         "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_DEBUG   "FALSE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
    >
    $<$<CONFIG:Release>:
        MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../maximilian"
    ">"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../JUCE/modules/juce_audio_processors/format_types/VST3_SDK;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../JuceLibraryCode;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../JUCE/modules"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "DEBUG;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "_CRT_SECURE_NO_WARNINGS;"
    "WIN32;"
    "_WINDOWS;"
    "JUCE_DISPLAY_SPLASH_SCREEN=1;"
    "JUCE_USE_DARK_SPLASH_SCREEN=1;"
    "JUCE_PROJUCER_VERSION=0x70003;"
    "JUCE_MODULE_AVAILABLE_juce_audio_basics=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_devices=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_formats=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_plugin_client=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_processors=1;"
    "JUCE_MODULE_AVAILABLE_juce_audio_utils=1;"
    "JUCE_MODULE_AVAILABLE_juce_core=1;"
    "JUCE_MODULE_AVAILABLE_juce_data_structures=1;"
    "JUCE_MODULE_AVAILABLE_juce_dsp=1;"
    "JUCE_MODULE_AVAILABLE_juce_events=1;"
    "JUCE_MODULE_AVAILABLE_juce_graphics=1;"
    "JUCE_MODULE_AVAILABLE_juce_gui_basics=1;"
    "JUCE_MODULE_AVAILABLE_juce_gui_extra=1;"
    "JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1;"
    "JUCE_VST3_CAN_REPLACE_VST2=0;"
    "JUCE_STRICT_REFCOUNTEDPOINTER=1;"
    "JucePlugin_Build_VST=0;"
    "JucePlugin_Build_VST3=1;"
    "JucePlugin_Build_AU=0;"
    "JucePlugin_Build_AUv3=0;"
    "JucePlugin_Build_AAX=0;"
    "JucePlugin_Build_Standalone=1;"
    "JucePlugin_Build_Unity=0;"
    "JucePlugin_Build_LV2=0;"
    "JucePlugin_Enable_IAA=0;"
    "JucePlugin_Enable_ARA=0;"
    "JucePlugin_Name=\"Synth\";"
    "JucePlugin_Desc=\"Synth\";"
    "JucePlugin_Manufacturer=\"yourcompany\";"
    "JucePlugin_ManufacturerWebsite=\"www.yourcompany.com\";"
    "JucePlugin_ManufacturerEmail=\"\";"
    "JucePlugin_ManufacturerCode=0x4d616e75;"
    "JucePlugin_PluginCode=0x43626b37;"
    "JucePlugin_IsSynth=1;"
    "JucePlugin_WantsMidiInput=1;"
    "JucePlugin_ProducesMidiOutput=1;"
    "JucePlugin_IsMidiEffect=0;"
    "JucePlugin_EditorRequiresKeyboardFocus=0;"
    "JucePlugin_Version=1.0.0;"
    "JucePlugin_VersionCode=0x10000;"
    "JucePlugin_VersionString=\"1.0.0\";"
    "JucePlugin_VSTUniqueID=JucePlugin_PluginCode;"
    "JucePlugin_VSTCategory=kPlugCategSynth;"
    "JucePlugin_Vst3Category=\"Instrument\";"
    "JucePlugin_AUMainType='aumu';"
    "JucePlugin_AUSubType=JucePlugin_PluginCode;"
    "JucePlugin_AUExportPrefix=SynthAU;"
    "JucePlugin_AUExportPrefixQuoted=\"SynthAU\";"
    "JucePlugin_AUManufacturerCode=JucePlugin_ManufacturerCode;"
    "JucePlugin_CFBundleIdentifier=com.yourcompany.Synth;"
    "JucePlugin_AAXIdentifier=com.yourcompany.Synth;"
    "JucePlugin_AAXManufacturerCode=JucePlugin_ManufacturerCode;"
    "JucePlugin_AAXProductId=JucePlugin_PluginCode;"
    "JucePlugin_AAXCategory=2048;"
    "JucePlugin_AAXDisableBypass=0;"
    "JucePlugin_AAXDisableMultiMono=0;"
    "JucePlugin_IAAType=0x61757269;"
    "JucePlugin_IAASubType=JucePlugin_PluginCode;"
    "JucePlugin_IAAName=\"yourcompany: Synth\";"
    "JucePlugin_VSTNumMidiInputs=16;"
    "JucePlugin_VSTNumMidiOutputs=16;"
    "JucePlugin_ARAContentTypes=0;"
    "JucePlugin_ARATransformationFlags=0;"
    "JucePlugin_ARAFactoryID=\"com.yourcompany.Synth.factory\";"
    "JucePlugin_ARADocumentArchiveID=\"com.yourcompany.Synth.aradocumentarchive.1.0.0\";"
    "JucePlugin_ARACompatibleArchiveIDs=\"\";"
    "JUCE_STANDALONE_APPLICATION=JucePlugin_Build_Standalone;"
    "JUCER_VS2022_78A503E=1;"
    "JUCE_APP_VERSION=1.0.0;"
    "JUCE_APP_VERSION_HEX=0x10000;"
    "JUCE_SHARED_CODE=1;"
    "_LIB"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Zi
        >
        $<$<CONFIG:Release>:
            /Ox;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /MP;
        /std:c++17;
        /W4;
        /GR;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /NODEFAULTLIB:libcmt.lib;
            /NODEFAULTLIB:msvcrt.lib
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /LTCG;
            /OPT:ICF
        >
        /NOLOGO;
        /MANIFEST;
        /SUBSYSTEM:WINDOWS
    )
endif()

