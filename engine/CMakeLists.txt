set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( GME_DIR "${PROJECT_SOURCE_DIR}/extlibs/gme-nes" )
set( GME_SOURCE_FILES 
    ${GME_DIR}/Blip_Buffer.cpp
    ${GME_DIR}/Classic_Emu.cpp
    ${GME_DIR}/Data_Reader.cpp
    ${GME_DIR}/Effects_Buffer.cpp
    ${GME_DIR}/gme.cpp
    ${GME_DIR}/Gme_File.cpp
    ${GME_DIR}/M3u_Playlist.cpp
    ${GME_DIR}/Multi_Buffer.cpp
    ${GME_DIR}/Music_Emu.cpp
    ${GME_DIR}/Nes_Apu.cpp
    ${GME_DIR}/Nes_Cpu.cpp
    ${GME_DIR}/Nes_Fme7_Apu.cpp
    ${GME_DIR}/Nes_Namco_Apu.cpp
    ${GME_DIR}/Nes_Oscs.cpp
    ${GME_DIR}/Nes_Vrc6_Apu.cpp
    ${GME_DIR}/Nsfe_Emu.cpp
    ${GME_DIR}/Nsf_Emu.cpp
)

set( JSONCPP_DIR "${PROJECT_SOURCE_DIR}/extlibs/jsoncpp" )
set( JSONCPP_SOURCE_FILES
    ${JSONCPP_DIR}/src/json_reader.cpp
    ${JSONCPP_DIR}/src/json_value.cpp
    ${JSONCPP_DIR}/src/json_writer.cpp
)

set( PHYSFS_STREAM_DIR "${PROJECT_SOURCE_DIR}/extlibs/physfs-stream" )
set( PHYSFS_STREAM_SOURCE_FILES
    ${PHYSFS_STREAM_DIR}/physfs/ifile_stream.cpp
    ${PHYSFS_STREAM_DIR}/physfs/ifile_streambuf.cpp
    ${PHYSFS_STREAM_DIR}/physfs/ofile_stream.cpp
    ${PHYSFS_STREAM_DIR}/physfs/ofile_streambuf.cpp
    ${PHYSFS_STREAM_DIR}/physfs/physfs_file_system.cpp
)

set( SQUIRREL_DIR "${PROJECT_SOURCE_DIR}/extlibs/squirrel" )
set( SQUIRREL_FILES
    # Squirrel core
    ${SQUIRREL_DIR}/squirrel/sqapi.cpp
    ${SQUIRREL_DIR}/squirrel/sqbaselib.cpp
    ${SQUIRREL_DIR}/squirrel/sqclass.cpp
    ${SQUIRREL_DIR}/squirrel/sqcompiler.cpp
    ${SQUIRREL_DIR}/squirrel/sqdebug.cpp
    ${SQUIRREL_DIR}/squirrel/sqfuncstate.cpp
    ${SQUIRREL_DIR}/squirrel/sqlexer.cpp
    ${SQUIRREL_DIR}/squirrel/sqmem.cpp
    ${SQUIRREL_DIR}/squirrel/sqobject.cpp
    ${SQUIRREL_DIR}/squirrel/sqstate.cpp
    ${SQUIRREL_DIR}/squirrel/sqtable.cpp
    ${SQUIRREL_DIR}/squirrel/sqvm.cpp

    # Squirrel standard library
    ${SQUIRREL_DIR}/sqstdlib/sqstdaux.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdblob.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdio.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdmath.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdrex.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdstream.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdstring.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdsystem.cpp
)

set( SQRAT_DIR "${PROJECT_SOURCE_DIR}/extlibs/sqrat" )

set( FASTDELEGATE_DIR "${PROJECT_SOURCE_DIR}/extlibs/fastdelegate" )

set( GUICHAN_DIR "${PROJECT_SOURCE_DIR}/extlibs/guichan" )
set( GUICHAN_SOURCE_FILES
    # Guichan core
    ${GUICHAN_DIR}/src/actionevent.cpp
    ${GUICHAN_DIR}/src/genericinput.cpp
    ${GUICHAN_DIR}/src/keyinput.cpp
    ${GUICHAN_DIR}/src/cliprectangle.cpp
    ${GUICHAN_DIR}/src/graphics.cpp
    ${GUICHAN_DIR}/src/mouseevent.cpp
    ${GUICHAN_DIR}/src/color.cpp
    ${GUICHAN_DIR}/src/gui.cpp
    ${GUICHAN_DIR}/src/mouseinput.cpp
    ${GUICHAN_DIR}/src/containerevent.cpp
    ${GUICHAN_DIR}/src/guichan.cpp
    ${GUICHAN_DIR}/src/rectangle.cpp
    ${GUICHAN_DIR}/src/defaultfont.cpp
    ${GUICHAN_DIR}/src/image.cpp
    ${GUICHAN_DIR}/src/selectionevent.cpp
    ${GUICHAN_DIR}/src/event.cpp
    ${GUICHAN_DIR}/src/imagefont.cpp
    ${GUICHAN_DIR}/src/text.cpp
    ${GUICHAN_DIR}/src/exception.cpp
    ${GUICHAN_DIR}/src/inputevent.cpp
    ${GUICHAN_DIR}/src/widget.cpp
    ${GUICHAN_DIR}/src/focushandler.cpp
    ${GUICHAN_DIR}/src/key.cpp
    ${GUICHAN_DIR}/src/font.cpp
    ${GUICHAN_DIR}/src/keyevent.cpp

    # Guichan widgets
    ${GUICHAN_DIR}/src/widgets/button.cpp
    ${GUICHAN_DIR}/src/widgets/checkbox.cpp
    ${GUICHAN_DIR}/src/widgets/container.cpp
    ${GUICHAN_DIR}/src/widgets/dropdown.cpp
    ${GUICHAN_DIR}/src/widgets/icon.cpp
    ${GUICHAN_DIR}/src/widgets/imagebutton.cpp
    ${GUICHAN_DIR}/src/widgets/label.cpp
    ${GUICHAN_DIR}/src/widgets/listbox.cpp
    ${GUICHAN_DIR}/src/widgets/radiobutton.cpp
    ${GUICHAN_DIR}/src/widgets/scrollarea.cpp
    ${GUICHAN_DIR}/src/widgets/slider.cpp
    ${GUICHAN_DIR}/src/widgets/tab.cpp
    ${GUICHAN_DIR}/src/widgets/tabbedarea.cpp
    ${GUICHAN_DIR}/src/widgets/textbox.cpp
    ${GUICHAN_DIR}/src/widgets/textfield.cpp
    ${GUICHAN_DIR}/src/widgets/window.cpp
)

set( GUICHAN_SFML_DIR "${PROJECT_SOURCE_DIR}/extlibs/guichan-sfml" )
set( GUICHAN_SFML_SOURCE_FILES
    ${GUICHAN_SFML_DIR}/src/guichan/sfml.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlfont.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlgraphics.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlimage.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlimageloader.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlinput.cpp
)

set( GUICHAN_HAKASE_DIR "${PROJECT_SOURCE_DIR}/extlibs/guichan-hakase" )
set( GUICHAN_HAKASE_SOURCE_FILES
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/fixedimagefont.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/functoractionlistener.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/functordeathlistener.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/functorfocuslistener.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/functorwidgetlistener.cpp
)

set( ENGINE_SOURCE_FILES
    src/hikari/client/ClientConfig.cpp
    src/hikari/client/CommandProcessor.cpp
    src/hikari/client/game/Effect.cpp
    src/hikari/client/game/events/BaseEventData.cpp
    src/hikari/client/game/events/EventData.cpp
    src/hikari/client/game/events/EventManager.cpp
    src/hikari/client/game/events/EventManagerImpl.cpp
    src/hikari/client/game/events/EntityDeathEventData.cpp
    src/hikari/client/game/events/EntityStateChangeEventData.cpp
    src/hikari/client/game/events/TransitionCollisionEventData.cpp
    src/hikari/client/game/events/WeaponFireEventData.cpp
    src/hikari/client/game/objects/brains/ScriptedEnemyBrain.cpp
    src/hikari/client/game/objects/CollectableItem.cpp
    src/hikari/client/game/objects/controllers/PlayerInputHeroActionController.cpp
    src/hikari/client/game/objects/effects/NothingEffect.cpp
    src/hikari/client/game/objects/effects/ScriptedEffect.cpp
    src/hikari/client/game/objects/Enemy.cpp
    src/hikari/client/game/objects/EnemyBrain.cpp
    src/hikari/client/game/objects/Entity.cpp
    src/hikari/client/game/objects/FactoryHelpers.cpp
    src/hikari/client/game/objects/HeroAirbornMobilityState.cpp
    src/hikari/client/game/objects/HeroClimbingMobilityState.cpp
    src/hikari/client/game/objects/HeroIdleMobilityState.cpp
    src/hikari/client/game/objects/HeroSlidingMobilityState.cpp
    src/hikari/client/game/objects/HeroTeleportingMobilityState.cpp
    src/hikari/client/game/objects/HeroWalkingMobilityState.cpp
    src/hikari/client/game/objects/ItemFactory.cpp
    src/hikari/client/game/objects/ItemSpawner.cpp
    src/hikari/client/game/objects/Spawner.cpp
    src/hikari/client/scripting/AudioServiceScriptProxy.cpp
    src/hikari/client/scripting/SquirrelService.cpp
    src/hikari/core/game/CollisionInfo.cpp
    src/hikari/core/game/Direction.cpp
    src/hikari/core/game/Movable.cpp
    src/hikari/core/game/TileMapCollisionResolver.cpp
    src/hikari/core/util/AnimationSetCache.cpp
    src/hikari/core/util/exception/HikariException.cpp
    src/hikari/core/util/exception/ServiceNotRegisteredException.cpp
    src/hikari/core/util/FileSystem.cpp
    src/hikari/core/util/HashedString.cpp
    src/hikari/core/util/Log.cpp
    src/hikari/core/util/Timer.cpp
    src/hikari/test/core/geom/Vector2DTest.cpp
    src/hikari/client/audio/AudioService.cpp
    src/hikari/client/game/GameWorld.cpp
    src/hikari/client/game/objects/Hero.cpp
    src/hikari/core/game/AnimationPlaybackException.cpp
    src/hikari/core/game/AnimationSet.cpp
    src/hikari/core/game/TileAnimator.cpp
    src/hikari/client/audio/NSFSoundStream.cpp
    src/hikari/client/Client.cpp
    src/hikari/client/game/GamePlayState.cpp
    src/hikari/client/game/GameProgress.cpp
    src/hikari/client/game/GuiTestState.cpp
    src/hikari/client/game/KeyboardInput.cpp
    src/hikari/client/game/MapTestState.cpp
    src/hikari/client/game/objects/GameObject.cpp
    src/hikari/client/game/RealTimeInput.cpp
    src/hikari/client/game/SpriteTestState.cpp
    src/hikari/client/game/StageSelectState.cpp
    src/hikari/client/gui/CommandConsole.cpp
    src/hikari/client/gui/EnergyMeter.cpp
    src/hikari/client/gui/Widget.cpp
    src/hikari/client/gui/HikariImageLoader.cpp
    src/hikari/client/gui/EnergyGauge.cpp
    src/hikari/core/game/Animation.cpp
    src/hikari/core/game/AnimationFrame.cpp
    src/hikari/core/game/AnimationLoader.cpp
    src/hikari/core/game/Animator.cpp
    src/hikari/core/game/GameController.cpp
    src/hikari/core/game/GameControllerException.cpp
    src/hikari/core/game/map/Camera.cpp
    src/hikari/core/game/map/Map.cpp
    src/hikari/core/game/map/MapLoader.cpp
    src/hikari/core/game/map/MapRenderer.cpp
    src/hikari/core/game/map/Room.cpp
    src/hikari/core/game/map/RoomTransition.cpp
    src/hikari/core/game/map/Tileset.cpp
    src/hikari/core/game/map/TilesetLoader.cpp
    src/hikari/core/game/SpriteAnimator.cpp
    src/hikari/core/gui/ImageFont.cpp
    src/hikari/core/Hikari.cpp
    src/hikari/core/math/NESNumber.cpp
    src/hikari/core/util/ImageCache.cpp
    src/hikari/core/util/JsonUtil.cpp
    src/hikari/core/util/PhysFS.cpp
    src/hikari/core/util/PhysFSUtils.cpp
    src/hikari/core/util/RedirectStream.cpp
    src/hikari/core/util/StringUtils.cpp
    src/hikari/core/util/TilesetCache.cpp
)

set( HIKARI_SOURCE_FILES
    ${GME_SOURCE_FILES}
    ${JSONCPP_SOURCE_FILES}
    ${PHYSFS_STREAM_SOURCE_FILES}
    ${SQUIRREL_FILES}
    ${GUICHAN_SOURCE_FILES}
    ${GUICHAN_SFML_SOURCE_FILES}
    ${GUICHAN_HAKASE_SOURCE_FILES}
    ${ENGINE_SOURCE_FILES}
)

set( INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/engine/include
    ${GME_DIR}
    ${JSONCPP_DIR}/include
    ${JSONCPP_DIR}/src
    ${PHYSFS_STREAM_DIR}
    ${SQUIRREL_DIR}/include
    ${SQUIRREL_DIR}/squirrel
    ${SQUIRREL_DIR}/sqstdlib
    ${SQRAT_DIR}/include
    ${FASTDELEGATE_DIR}
    ${GUICHAN_DIR}/include
    ${GUICHAN_DIR}/include/guichan
    ${GUICHAN_SFML_DIR}/include
    ${GUICHAN_HAKASE_DIR}/include
)

include_directories( ${INCLUDE_DIRS} )

add_executable( hikari ${HIKARI_SOURCE_FILES} ${INCLUDE_DIRS} )

#
# We want to link statically on Windows
#
if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
endif(WIN32)

find_package(SFML 2 COMPONENTS graphics window system audio REQUIRED)

if(SFML_FOUND)
    if(APPLE)
        set(OTHER_SFML_LDFLAGS ${OTHER_SFML_LDFLAGS} "-stdlib=libc++")
    endif(APPLE)
    include_directories( ${SFML_INCLUDE_DIR} )
    target_link_libraries( hikari ${SFML_LIBRARIES} ${OTHER_SFML_LDFLAGS} )
endif(SFML_FOUND)

find_package(PhysFS REQUIRED)

if(PHYSFS_FOUND)
    ADD_DEFINITIONS(-fno-common)
    if(APPLE)
        set(OTHER_LDFLAGS ${OTHER_LDFLAGS} "-framework Carbon -framework IOKit")
    endif(APPLE)
    include_directories( ${PHYSFS_INCLUDE_DIR} )
    target_link_libraries( hikari ${PHYSFS_LIBRARY} ${OTHER_LDFLAGS} )
endif(PHYSFS_FOUND)