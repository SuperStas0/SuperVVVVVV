/* Lists every member of Game, mapclass, entityclass, scriptclass and GfxState
 * with its nested types. Made from the class layout, so it needs a new
 * version when members are added or removed. */

#include "SavestateIO.h"

#include "Entity.h"
#include "Game.h"
#include "Map.h"
#include "Script.h"
#include "Textbox.h"
#include "TowerBG.h"

namespace savestate_io
{
    static const unsigned int FIELDS_HASH = 0x73c8c237u;

    unsigned int fields_hash(void)
    {
        return FIELDS_HASH;
    }

    static void io(Stream& s, Game::Summary& v);
    static void io(Stream& s, MenuOption& v);
    static void io(Stream& s, MenuStackFrame& v);
    static void io(Stream& s, SDL_Rect& v);
    static void io(Stream& s, Roomname& v);
    static void io(Stream& s, SDL_Point& v);
    static void io(Stream& s, Roomtext& v);
    static void io(Stream& s, otherlevelclass& v);
    static void io(Stream& s, spacestation2class& v);
    static void io(Stream& s, labclass& v);
    static void io(Stream& s, finalclass& v);
    static void io(Stream& s, warpclass& v);
    static void io(Stream& s, towerclass& v);
    static void io(Stream& s, entclass& v);
    static void io(Stream& s, SDL_Color& v);
    static void io(Stream& s, blockclass& v);
    static void io(Stream& s, TextboxCrewmatePosition& v);
    static void io(Stream& s, TextboxOriginalContext& v);
    static void io(Stream& s, TextboxSprite& v);
    static void io(Stream& s, Script& v);
    static void io(Stream& s, TowerBG& v);
    static void io(Stream& s, textboxclass& v);
    static void io(Stream& s, TextboxSpacing& v);
    static void io(Stream& s, FadeBars& v);
    static void io(Stream& s, GameGamestate& v);
    static void io(Stream& s, Menu::MenuName& v);
    static void io(Stream& s, RoomnameType& v);
    static void io(Stream& s, SDL_GameControllerButton& v);
    static void io(Stream& s, SLIDERMODE& v);
    static void io(Stream& s, SWNMODE& v);
    static void io(Stream& s, TextboxImage& v);
    static void io(Stream& s, TextboxTranslate& v);
    static void io(Stream& s, std::vector<textboxclass>& v);

    static void io(Stream& s, std::vector<textboxclass>& v)
    {
        unsigned int count = (unsigned int) v.size();
        io(s, count);
        if (s.reading())
        {
            if (count > s.remaining())
            {
                s.fail();
                return;
            }
            v.clear();
            v.resize(count, textboxclass(0));
        }
        for (size_t i = 0; i < v.size() && s.ok(); i++)
        {
            io(s, v[i]);
        }
    }

    static void io(Stream& s, FadeBars& v)
    {
        io_enum(s, v);
    }

    static void io(Stream& s, GameGamestate& v)
    {
        io_enum(s, v);
    }

    static void io(Stream& s, Menu::MenuName& v)
    {
        io_enum(s, v);
    }

    static void io(Stream& s, RoomnameType& v)
    {
        io_enum(s, v);
    }

    static void io(Stream& s, SDL_GameControllerButton& v)
    {
        io_enum(s, v);
    }

    static void io(Stream& s, SLIDERMODE& v)
    {
        io_enum(s, v);
    }

    static void io(Stream& s, SWNMODE& v)
    {
        io_enum(s, v);
    }

    static void io(Stream& s, TextboxImage& v)
    {
        io_enum(s, v);
    }

    static void io(Stream& s, TextboxTranslate& v)
    {
        io_enum(s, v);
    }

    void io(Stream& s, Game& v)
    {
        io(s, v.last_telesave);
        io(s, v.last_quicksave);
        io(s, v.roomx);
        io(s, v.roomy);
        io(s, v.prevroomx);
        io(s, v.prevroomy);
        io(s, v.savex);
        io(s, v.savey);
        io(s, v.saverx);
        io(s, v.savery);
        io(s, v.savegc);
        io(s, v.savedir);
        io(s, v.savecolour);
        io(s, v.edsavex);
        io(s, v.edsavey);
        io(s, v.edsaverx);
        io(s, v.edsavery);
        io(s, v.edsavegc);
        io(s, v.edsavedir);
        io(s, v.state);
        io(s, v.statedelay);
        io(s, v.glitchrunkludge);
        io(s, v.gamestate);
        io(s, v.prevgamestate);
        io(s, v.hascontrol);
        io(s, v.jumpheld);
        io(s, v.jumppressed);
        io(s, v.gravitycontrol);
        io(s, v.muted);
        io(s, v.mutebutton);
        io(s, v.musicmuted);
        io(s, v.musicmutebutton);
        io(s, v.tapleft);
        io(s, v.tapright);
        io(s, v.mapheld);
        io(s, v.menupage);
        io(s, v.lastsaved);
        io(s, v.deathcounts);
        io(s, v.framecounter);
        io(s, v.seed_use_sdl_getticks);
        io(s, v.editor_disabled);
        io(s, v.frames);
        io(s, v.seconds);
        io(s, v.minutes);
        io(s, v.hours);
        io(s, v.gamesaved);
        io(s, v.gamesavefailed);
        io(s, v.savetime);
        io(s, v.saveframes);
        io(s, v.saveseconds);
        io(s, v.savetrinkets);
        io(s, v.startscript);
        io(s, v.newscript);
        io(s, v.menustart);
        io(s, v.teleport_to_new_area);
        io(s, v.teleport_to_x);
        io(s, v.teleport_to_y);
        io(s, v.teleportscript);
        io(s, v.useteleporter);
        io(s, v.teleport_to_teleporter);
        io(s, v.menuoptions);
        io(s, v.currentmenuoption);
        io(s, v.menutestmode);
        io(s, v.currentmenuname);
        io(s, v.kludge_ingametemp);
        io(s, v.slidermode);
        io(s, v.current_credits_list_index);
        io(s, v.translator_credits_pagenum);
        io(s, v.menuxoff);
        io(s, v.menuyoff);
        io(s, v.menuspacing);
        io(s, v.menustack);
        io(s, v.menucountdown);
        io(s, v.menudest);
        io(s, v.creditposx);
        io(s, v.creditposy);
        io(s, v.creditposdelay);
        io(s, v.oldcreditposx);
        io(s, v.gpmenu_lastbutton);
        io(s, v.gpmenu_confirming);
        io(s, v.gpmenu_showremove);
        io(s, v.silence_settings_error);
        io(s, v.swnmode);
        io(s, v.swngame);
        io(s, v.swnstate);
        io(s, v.swnstate2);
        io(s, v.swnstate3);
        io(s, v.swnstate4);
        io(s, v.swndelay);
        io(s, v.swndeaths);
        io(s, v.swntimer);
        io(s, v.swncolstate);
        io(s, v.swncoldelay);
        io(s, v.swnrecord);
        io(s, v.swnbestrank);
        io(s, v.swnrank);
        io(s, v.swnmessage);
        io(s, v.supercrewmate);
        io(s, v.scmhurt);
        io(s, v.scmprogress);
        io(s, v.colourblindmode);
        io(s, v.noflashingmode);
        io(s, v.slowdown);
        io(s, v.nodeathmode);
        io(s, v.gameoverdelay);
        io(s, v.nocutscenes);
        io(s, v.ndmresultcrewrescued);
        io(s, v.ndmresulttrinkets);
        io(s, v.ndmresulthardestroom);
        io(s, v.ndmresulthardestroom_x);
        io(s, v.ndmresulthardestroom_y);
        io(s, v.ndmresulthardestroom_specialname);
        io(s, v.intimetrial);
        io(s, v.timetrialparlost);
        io(s, v.timetrialcountdown);
        io(s, v.timetrialshinytarget);
        io(s, v.timetriallevel);
        io(s, v.timetrialpar);
        io(s, v.timetrialresulttime);
        io(s, v.timetrialresultframes);
        io(s, v.timetrialrank);
        io(s, v.timetrialcheater);
        io(s, v.timetrialresultshinytarget);
        io(s, v.timetrialresulttrinkets);
        io(s, v.timetrialresultpar);
        io(s, v.timetrialresultdeaths);
        io(s, v.start_translator_exploring);
        io(s, v.translator_exploring);
        io(s, v.translator_exploring_allowtele);
        io(s, v.translator_cutscene_test);
        io(s, v.cutscenetest_menu_page);
        io(s, v.cutscenetest_menu_play_id);
        io(s, v.creditposition);
        io(s, v.oldcreditposition);
        io(s, v.insecretlab);
        io(s, v.inintermission);
        io(s, v.crewstats);
        io(s, v.ndmresultcrewstats);
        io(s, v.alarmon);
        io(s, v.alarmdelay);
        io(s, v.blackout);
        io(s, v.unlock);
        io(s, v.unlocknotify);
        io(s, v.stat_trinkets);
        io(s, v.bestgamedeaths);
        io(s, v.besttimes);
        io(s, v.bestframes);
        io(s, v.besttrinkets);
        io(s, v.bestlives);
        io(s, v.bestrank);
        io(s, v.screenshake);
        io(s, v.flashlight);
        io(s, v.advancetext);
        io(s, v.pausescript);
        io(s, v.deathseq);
        io(s, v.lifeseq);
        io(s, v.savepoint);
        io(s, v.teleportxpos);
        io(s, v.teleport);
        io(s, v.edteleportent);
        io(s, v.completestop);
        io(s, v.inertia);
        io(s, v.companion);
        io(s, v.teleblock);
        io(s, v.activetele);
        io(s, v.readytotele);
        io(s, v.oldreadytotele);
        io(s, v.activity_r);
        io(s, v.activity_g);
        io(s, v.activity_b);
        io(s, v.activity_y);
        io(s, v.activity_lastprompt);
        io(s, v.activity_gettext);
        io(s, v.backgroundtext);
        io(s, v.activeactivity);
        io(s, v.act_fade);
        io(s, v.prev_act_fade);
        io(s, v.press_left);
        io(s, v.press_right);
        io(s, v.press_action);
        io(s, v.press_map);
        io(s, v.press_interact);
        io(s, v.interactheld);
        io(s, v.separate_interact);
        io(s, v.totalflips);
        io(s, v.hardestroom);
        io(s, v.hardestroomdeaths);
        io(s, v.currentroomdeaths);
        io(s, v.hardestroom_x);
        io(s, v.hardestroom_y);
        io(s, v.hardestroom_specialname);
        io(s, v.hardestroom_finalstretch);
        io(s, v.quickrestartkludge);
        io(s, v.customscript);
        io(s, v.customcol);
        io(s, v.levelpage);
        io(s, v.playcustomlevel);
        io(s, v.customleveltitle);
        io(s, v.customlevelfilename);
        io(s, v.customlevelstats);
        io(s, v.controllerButton_map);
        io(s, v.controllerButton_flip);
        io(s, v.controllerButton_esc);
        io(s, v.controllerButton_restart);
        io(s, v.controllerButton_interact);
        io(s, v.skipfakeload);
        io(s, v.ghostsenabled);
        io(s, v.cliplaytest);
        io(s, v.playx);
        io(s, v.playy);
        io(s, v.playrx);
        io(s, v.playry);
        io(s, v.playgc);
        io(s, v.playmusic);
        io(s, v.playassets);
        io(s, v.fadetomenu);
        io(s, v.fadetomenudelay);
        io(s, v.fadetolab);
        io(s, v.fadetolabdelay);
        io(s, v.over30mode);
        io(s, v.showingametimer);
        io(s, v.ingame_titlemode);
        io(s, v.ingame_editormode);
        io(s, v.disablepause);
        io(s, v.disableaudiopause);
        io(s, v.disabletemporaryaudiopause);
        io(s, v.inputdelay);
        io(s, v.statelocked);
        io(s, v.old_skip_message_timer);
        io(s, v.skip_message_timer);
        io(s, v.old_mode_indicator_timer);
        io(s, v.mode_indicator_timer);
        io(s, v.old_screenshot_border_timer);
        io(s, v.screenshot_border_timer);
        io(s, v.screenshot_saved_success);
    }

    static void io(Stream& s, Game::Summary& v)
    {
        io(s, v.exists);
        io(s, v.seconds);
        io(s, v.minutes);
        io(s, v.hours);
        io(s, v.saverx);
        io(s, v.savery);
        io(s, v.trinkets);
        io(s, v.crewstats);
    }

    static void io(Stream& s, MenuOption& v)
    {
        io(s, v.text);
        io(s, v.active);
        io(s, v.print_flags);
    }

    static void io(Stream& s, MenuStackFrame& v)
    {
        io(s, v.option);
        io(s, v.name);
    }

    static void io(Stream& s, SDL_Rect& v)
    {
        io(s, v.x);
        io(s, v.y);
        io(s, v.w);
        io(s, v.h);
    }

    void io(Stream& s, mapclass& v)
    {
        io(s, v.nexttowercolour_set);
        io(s, v.roomdeaths);
        io(s, v.roomdeathsfinal);
        io(s, v.contents);
        io(s, v.explored);
        io(s, v.background);
        io(s, v.rcol);
        io(s, v.tileset);
        io(s, v.warpx);
        io(s, v.warpy);
        io(s, v.roomname_special);
        io(s, v.roomnameset);
        io_cstr(s, v.hiddenname);
        io(s, v.specialroomnames);
        io(s, v.towermode);
        io(s, v.ypos);
        io(s, v.oldypos);
        io(s, v.cameramode);
        io(s, v.cameraseek);
        io(s, v.cameraseekframe);
        io(s, v.resumedelay);
        io(s, v.minitowermode);
        io(s, v.colstatedelay);
        io(s, v.colsuperstate);
        io(s, v.spikeleveltop);
        io(s, v.spikelevelbottom);
        io(s, v.oldspikeleveltop);
        io(s, v.oldspikelevelbottom);
        io(s, v.finalmode);
        io(s, v.finalstretch);
        io(s, v.custommode);
        io(s, v.custommodeforreal);
        io(s, v.custommmxoff);
        io(s, v.custommmyoff);
        io(s, v.custommmxsize);
        io(s, v.custommmysize);
        io(s, v.customzoom);
        io(s, v.customshowmm);
        io(s, v.final_colormode);
        io(s, v.final_mapcol);
        io(s, v.final_aniframe);
        io(s, v.final_aniframedelay);
        io(s, v.final_colorframe);
        io(s, v.final_colorframedelay);
        io(s, v.teleporters);
        io(s, v.shinytrinkets);
        io(s, v.showteleporters);
        io(s, v.showtargets);
        io(s, v.showtrinkets);
        io(s, v.roomtexton);
        io(s, v.roomtext);
        io(s, v.otherlevel);
        io(s, v.spacestation2);
        io(s, v.lablevel);
        io(s, v.finallevel);
        io(s, v.warplevel);
        io(s, v.tower);
        io(s, v.extrarow);
        io(s, v.invincibility);
        io(s, v.cursorstate);
        io(s, v.cursordelay);
    }

    static void io(Stream& s, Roomname& v)
    {
        io(s, v.x);
        io(s, v.y);
        io(s, v.loop);
        io(s, v.flag);
        io(s, v.type);
        io(s, v.text);
        io(s, v.progress);
        io(s, v.delay);
    }

    static void io(Stream& s, SDL_Point& v)
    {
        io(s, v.x);
        io(s, v.y);
    }

    static void io(Stream& s, Roomtext& v)
    {
        io(s, v.x);
        io(s, v.y);
        io_cstr(s, v.text);
        io(s, v.rtl);
    }

    static void io(Stream& s, otherlevelclass& v)
    {
        io_cstr(s, v.roomname);
        io(s, v.roomname_special);
        io_cstr(s, v.hiddenname);
        io(s, v.roomtileset);
    }

    static void io(Stream& s, spacestation2class& v)
    {
        io_cstr(s, v.roomname);
        io(s, v.roomname_special);
    }

    static void io(Stream& s, labclass& v)
    {
        io_cstr(s, v.roomname);
        io(s, v.roomname_special);
        io(s, v.rcol);
    }

    static void io(Stream& s, finalclass& v)
    {
        io_cstr(s, v.roomname);
        io(s, v.roomname_special);
        io(s, v.warpx);
        io(s, v.warpy);
    }

    static void io(Stream& s, warpclass& v)
    {
        io_cstr(s, v.roomname);
        io(s, v.roomname_special);
        io(s, v.rcol);
        io(s, v.warpx);
        io(s, v.warpy);
    }

    static void io(Stream& s, towerclass& v)
    {
        io(s, v.back);
        io(s, v.contents);
        io(s, v.minitower);
        io(s, v.minitowermode);
    }

    void io(Stream& s, entityclass& v)
    {
        io(s, v.entities);
        io(s, v.k);
        io(s, v.blocks);
        io(s, v.flags);
        io(s, v.collect);
        io(s, v.customcollect);
        io(s, v.platformtile);
        io(s, v.vertplatforms);
        io(s, v.horplatforms);
        io(s, v.nearelephant);
        io(s, v.upsetmode);
        io(s, v.upset);
        io(s, v.trophytext);
        io(s, v.trophytype);
        io(s, v.oldtrophytext);
        io(s, v.altstates);
        io(s, v.customenemy);
        io(s, v.customplatformtile);
        io(s, v.customwarpmode);
        io(s, v.customwarpmodevon);
        io(s, v.customwarpmodehon);
        io(s, v.customscript);
        io(s, v.customcrewmoods);
        io(s, v.customactivitycolour);
        io(s, v.customactivitytext);
        io(s, v.customactivitypositiony);
    }

    static void io(Stream& s, entclass& v)
    {
        io(s, v.invis);
        io(s, v.type);
        io(s, v.size);
        io(s, v.tile);
        io(s, v.rule);
        io(s, v.state);
        io(s, v.statedelay);
        io(s, v.behave);
        io(s, v.animate);
        io(s, v.para);
        io(s, v.life);
        io(s, v.colour);
        io(s, v.oldxp);
        io(s, v.oldyp);
        io(s, v.ax);
        io(s, v.ay);
        io(s, v.vx);
        io(s, v.vy);
        io(s, v.cx);
        io(s, v.cy);
        io(s, v.w);
        io(s, v.h);
        io(s, v.newxp);
        io(s, v.newyp);
        io(s, v.isplatform);
        io(s, v.x1);
        io(s, v.y1);
        io(s, v.x2);
        io(s, v.y2);
        io(s, v.onentity);
        io(s, v.harmful);
        io(s, v.onwall);
        io(s, v.onxwall);
        io(s, v.onywall);
        io(s, v.gravity);
        io(s, v.onground);
        io(s, v.onroof);
        io(s, v.framedelay);
        io(s, v.drawframe);
        io(s, v.walkingframe);
        io(s, v.dir);
        io(s, v.actionframe);
        io(s, v.collisionframedelay);
        io(s, v.collisiondrawframe);
        io(s, v.collisionwalkingframe);
        io(s, v.visualonground);
        io(s, v.visualonroof);
        io(s, v.yp);
        io(s, v.xp);
        io(s, v.realcol);
        io(s, v.lerpoldxp);
        io(s, v.lerpoldyp);
    }

    static void io(Stream& s, SDL_Color& v)
    {
        io(s, v.r);
        io(s, v.g);
        io(s, v.b);
        io(s, v.a);
    }

    static void io(Stream& s, blockclass& v)
    {
        io(s, v.rect);
        io(s, v.type);
        io(s, v.trigger);
        io(s, v.xp);
        io(s, v.yp);
        io(s, v.wp);
        io(s, v.hp);
        io(s, v.script);
        io(s, v.prompt);
        io(s, v.r);
        io(s, v.g);
        io(s, v.b);
        io(s, v.activity_y);
        io(s, v.gettext);
    }

    void io(Stream& s, scriptclass& v)
    {
        io(s, v.commands);
        io(s, v.words);
        io(s, v.txt);
        io(s, v.scriptname);
        io(s, v.position);
        io(s, v.looppoint);
        io(s, v.loopcount);
        io(s, v.scriptdelay);
        io(s, v.running);
        io(s, v.textbox_colours);
        io(s, v.textx);
        io(s, v.texty);
        io(s, v.textcrewmateposition);
        io(s, v.textoriginalcontext);
        io(s, v.r);
        io(s, v.g);
        io(s, v.b);
        io(s, v.textflipme);
        io(s, v.textcase);
        io(s, v.textbuttons);
        io(s, v.textlarge);
        io(s, v.textboxtimer);
        io(s, v.textbox_sprites);
        io(s, v.textbox_image);
        io(s, v.i);
        io(s, v.j);
        io(s, v.k);
        io(s, v.customscripts);
    }

    static void io(Stream& s, TextboxCrewmatePosition& v)
    {
        io(s, v.override_x);
        io(s, v.override_y);
        io(s, v.x);
        io(s, v.y);
        io(s, v.dir);
        io(s, v.text_above);
    }

    static void io(Stream& s, TextboxOriginalContext& v)
    {
        io(s, v.x);
        io(s, v.y);
        io(s, v.lines);
        io(s, v.script_name);
        io(s, v.text_case);
    }

    static void io(Stream& s, TextboxSprite& v)
    {
        io(s, v.x);
        io(s, v.y);
        io(s, v.col);
        io(s, v.tile);
    }

    static void io(Stream& s, Script& v)
    {
        io(s, v.name);
        io(s, v.contents);
    }

    void io(Stream& s, savestate::GfxState& v)
    {
        io(s, v.rcol);
        io(s, v.towerbg);
        io(s, v.fademode);
        io(s, v.fadeamount);
        io(s, v.oldfadeamount);
        io(s, v.fadebars);
        io(s, v.ingame_fademode);
        io(s, v.showcutscenebars);
        io(s, v.cutscenebarspos);
        io(s, v.oldcutscenebarspos);
        io(s, v.trinketcolset);
        io(s, v.trinketr);
        io(s, v.trinketg);
        io(s, v.trinketb);
        io(s, v.linestate);
        io(s, v.linedelay);
        io(s, v.backoffset);
        io(s, v.textboxes);
    }

    static void io(Stream& s, TowerBG& v)
    {
        io(s, v.tdrawback);
        io(s, v.bypos);
        io(s, v.bscroll);
        io(s, v.colstate);
        io(s, v.scrolldir);
        io(s, v.r);
        io(s, v.g);
        io(s, v.b);
    }

    static void io(Stream& s, textboxclass& v)
    {
        io(s, v.lines);
        io(s, v.xp);
        io(s, v.yp);
        io(s, v.w);
        io(s, v.h);
        io(s, v.r);
        io(s, v.g);
        io(s, v.b);
        io(s, v.linegap);
        io(s, v.timer);
        io(s, v.tl);
        io(s, v.prev_tl);
        io(s, v.tm);
        io(s, v.flipme);
        io(s, v.rand);
        io(s, v.large);
        io(s, v.should_centerx);
        io(s, v.should_centery);
        io(s, v.print_flags);
        io(s, v.translate);
        io(s, v.fill_buttons);
        io(s, v.sprites);
        io(s, v.image);
        io(s, v.crewmate_position);
        io(s, v.original);
        io(s, v.spacing);
        io(s, v.other_textbox_index);
        if (s.reading())
        {
            v.function = NULL;
            if (v.translate == TEXTTRANSLATE_FUNCTION)
            {
                v.translate = TEXTTRANSLATE_NONE;
            }
        }
    }

    static void io(Stream& s, TextboxSpacing& v)
    {
        io(s, v.centertext);
        io(s, v.pad_left);
        io(s, v.pad_right);
        io(s, v.padtowidth);
    }
}
