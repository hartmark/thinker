
#include "gui.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"


// Size in pixels of the bottom middle UI console
const int ConsoleHeight = 219;
const int ConsoleWidth = 1024;

struct ConsoleState {
    const int ScrollMin = 1;
    const int ScrollMax = 20;
    const int ListScrollDelta = 1;
    POINT ScreenSize;
    bool MouseOverTileInfo = true;
    bool TimersEnabled = false;
    bool Scrolling = false;
    bool RightButtonDown = false;
    bool ScrollDragging = false;
    double ScrollOffsetX = 0.0;
    double ScrollOffsetY = 0.0;
    POINT ScrollDragPos = {0, 0};
} CState;

/*
The following lists contain definitions copied directly from PRACX (e.g. functions with _F suffix).
These are mostly provided for reference and using them should be avoided because the names should be
converted to the actual names reversed from the SMACX binary (add F prefix for function prototypes).
*/

typedef int(__stdcall *START_F)(HINSTANCE, HINSTANCE, LPSTR, int);
typedef int(__thiscall *CCANVAS_CREATE_F)(Buffer* pthis);
typedef int(__stdcall *WNDPROC_F)(HWND, int, WPARAM, LPARAM);
typedef int(__thiscall *CMAIN_ZOOMPROCESSING_F)(Console* pthis);
typedef int(__stdcall *PROC_ZOOM_KEY_F)(int iZoomType, int iZero);
typedef int(__thiscall *CMAIN_TILETOPT_F)(Console* pthis, int iTileX, int iTileY, long* piX, long* piY);
typedef int(__thiscall *CMAIN_MOVEMAP_F)(Console *pthis, int iXPos, int iYPos, int a4);
typedef int(__thiscall *CMAIN_REDRAWMAP_F)(Console *pthis, int a2);
typedef int(__thiscall *CMAIN_DRAWMAP_F)(Console* This, int iOwner, int fUnitsOnly);
typedef int(__thiscall *CMAIN_PTTOTILE_F)(Console* This, POINT p, long* piTileX, long* piTileY);
typedef int(__thiscall *CINFOWIN_DRAWTILEINFO_F)(StatusWin* This);
typedef int(__cdecl *PAINTHANDLER_F)(RECT *prRect, int a2);
typedef int(__cdecl *PAINTMAIN_F)(RECT *pRect);
typedef int(__thiscall *CSPRITE_FROMCANVASRECTTRANS_F)(Sprite *This, Buffer *poCanvas,
    int iTransparentIndex, int iLeft, int iTop, int iWidth, int iHeight, int a8);
typedef int(__thiscall *CCANVAS_DESTROY4_F)(Buffer *This);
typedef int (__thiscall *CSPRITE_STRETCHCOPYTOCANVAS_F)
    (Sprite *This, Buffer *poCanvasDest, int cTransparentIndex, int iLeft, int iTop);
typedef int(__thiscall *CSPRITE_STRETCHCOPYTOCANVAS1_F)
    (Sprite *This, Buffer *poCanvasDest, int cTransparentIndex, int iLeft, int iTop, int iDestScale, int iSourceScale);
typedef int (__thiscall *CSPRITE_STRETCHDRAWTOCANVAS2_F)
    (Sprite *This, Buffer *poCanvas, int a1, int a2, int a3, int a4, int a7, int a8);
typedef int(__thiscall *CWINBASE_ISVISIBLE_F)(Win *This);
typedef int(__thiscall *CTIMER_STARTTIMER_F)(Time *This, int a2, int a3, int iDelay, int iElapse, int uResolution);
typedef int(__thiscall *CTIMER_STOPTIMER_F)(Time *This);
typedef int(__thiscall *DRAWCITYMAP_F)(Win *This, int a2);
typedef int(__cdecl *GETFOODCOUNT_F)(int iForFaction, int a2, int iTileX, int iTileY, bool fWithFarm);
typedef int(__cdecl *GETPRODCOUNT_F)(int iForFaction, int a2, int iTileX, int iTileY, bool fWithMine);
typedef int(__cdecl *GETENERGYCOUNT_F)(int iForFaction, int a2, int iTileX, int iTileY, bool fWithSolar);
typedef int(__thiscall *IMAGEFROMCANVAS_F)(CImage *This, Buffer *poCanvasSource, int iLeft, int iTop, int iWidth, int iHeight, int a7);
typedef int(__cdecl *GETELEVATION_F)(int iTileX, int iTileY);
typedef int(__thiscall *CIMAGE_COPYTOCANVAS2_F)(CImage *This, Buffer *poCanvasDest, int x, int y, int a5, int a6, int a7);
typedef int(__thiscall *CMAINMENU_ADDSUBMENU_F)(Menu* This, int iMenuID, int iMenuItemID, char *lpString);
typedef int(__thiscall *CMAINMENU_ADDBASEMENU_F)(Menu *This, int iMenuItemID, const char *pszCaption, int a4);
typedef int(__thiscall *CMAINMENU_ADDSEPARATOR_F)(Menu *This, int iMenuItemID, int iSubMenuItemID);
typedef int(__thiscall *CMAINMENU_UPDATEVISIBLE_F)(Menu *This, int a2);
typedef int(__thiscall *CMAINMENU_RENAMEMENUITEM_F)(Menu *This, int iMenuItemID, int iSubMenuItemID, const char *pszCaption);
typedef long(__thiscall *CMAP_GETCORNERYOFFSET_F)(MapWin_Alt *This, int iTileX, int iTileY, int iCorner);

START_F                        pfncWinMain =                    (START_F                       )0x45F950;
HDC*                           phDC =                           (HDC*                          )0x9B7B2C;
HWND*                          phWnd =                          (HWND*                         )0x9B7B28;
HPALETTE*                      phPallete =                      (HPALETTE*                     )0x9B8178;
HINSTANCE*                     phInstance =                     (HINSTANCE*                    )0x9B7B14;
WNDPROC_F                      pfncWinProc =                    (WNDPROC_F                     )0x5F0650;
CMAIN_ZOOMPROCESSING_F         pfncZoomProcessing =             (CMAIN_ZOOMPROCESSING_F        )0x462980;
Console*                       pMain =                          (Console*                      )0x9156B0;
int*                           piMaxTileX =                     (int*                          )0x949870;
int*                           piMaxTileY =                     (int*                          )0x949874;
PROC_ZOOM_KEY_F                pfncProcZoomKey =                (PROC_ZOOM_KEY_F               )0x5150D0;
CMAIN_TILETOPT_F               pfncTileToPoint =                (CMAIN_TILETOPT_F              )0x462F00;
RECT*                          prVisibleTiles =                 (RECT*                         )0x7D3C28;
int*                           piMapFlags =                     (int*                          )0x94988C;
CMAIN_MOVEMAP_F                pfncMoveMap =                    (CMAIN_MOVEMAP_F               )0x46B1F0;
CMAIN_REDRAWMAP_F              pfncRedrawMap =                  (CMAIN_REDRAWMAP_F             )0x46A550;
CMAIN_DRAWMAP_F                pfncDrawMap =                    (CMAIN_DRAWMAP_F               )0x469CA0;
CMAIN_PTTOTILE_F               pfncPtToTile =                   (CMAIN_PTTOTILE_F              )0x463040;
StatusWin*                     pInfoWin =                       (StatusWin*                    )0x8C5568;
CINFOWIN_DRAWTILEINFO_F        pfncDrawTileInfo =               (CINFOWIN_DRAWTILEINFO_F       )0x4B8890; // Fixed
Console**                      ppMain =                         (Console**                     )0x7D3C3C;
PAINTHANDLER_F                 pfncPaintHandler =               (PAINTHANDLER_F                )0x5F7320;
PAINTMAIN_F                    pfncPaintMain =                  (PAINTMAIN_F                   )0x5EFD20;
CSPRITE_FROMCANVASRECTTRANS_F  pfncSpriteFromCanvasRectTrans =  (CSPRITE_FROMCANVASRECTTRANS_F )0x5E39A0;
Buffer*                        poLoadingCanvas =                (Buffer*                       )0x798668;
CCANVAS_DESTROY4_F             pfncCanvasDestroy4 =             (CCANVAS_DESTROY4_F            )0x5D7470;
CSPRITE_STRETCHCOPYTOCANVAS_F  pfncSpriteStretchCopyToCanvas =  (CSPRITE_STRETCHCOPYTOCANVAS_F )0x5E4B9A;
CSPRITE_STRETCHCOPYTOCANVAS1_F pfncSpriteStretchCopyToCanvas1 = (CSPRITE_STRETCHCOPYTOCANVAS1_F)0x5E4B4A;
CSPRITE_STRETCHDRAWTOCANVAS2_F pfncSpriteStretchDrawToCanvas2 = (CSPRITE_STRETCHDRAWTOCANVAS2_F)0x5E3E00;
Sprite*                        pSprResourceIcons =              (Sprite*                       )0x7A72A0;
Win*                           pCityWindow =                    (Win*                          )0x6A7628;
Win*                           pAnotherWindow =                 (Win*                          )0x8A6270;
Win*                           pAnotherWindow2 =                (Win*                          )0x8C6E68;
int*                           pfGameNotStarted =               (int*                          )0x68F21C;
CWINBASE_ISVISIBLE_F           pfncWinIsVisible =               (CWINBASE_ISVISIBLE_F          )0x5F7E90;
CTIMER_STARTTIMER_F            pfncStartTimer =                 (CTIMER_STARTTIMER_F           )0x616350;
CTIMER_STOPTIMER_F             pfncStopTimer =                  (CTIMER_STOPTIMER_F            )0x616780;
DRAWCITYMAP_F                  pfncDrawCityMap =                (DRAWCITYMAP_F                 )0x40F0F0;
int*                           piZoomNum =                      (int*                          )0x691E6C;
int*                           piZoomDenom =                    (int*                          )0x691E70;
int*                           piSourceScale =                  (int*                          )0x696D1C;
int*                           piDestScale =                    (int*                          )0x696D18;
int*                           piResourceExtra =                (int*                          )0x90E998;
int*                           piTilesPerRow =                  (int*                          )0x68FAF0;
CTile**                        paTiles =                        (CTile**                       )0x94A30C;
GETFOODCOUNT_F                 pfncGetFoodCount =               (GETFOODCOUNT_F                )0x4E6E50;
GETPRODCOUNT_F                 pfncGetProdCount =               (GETPRODCOUNT_F                )0x4E7310;
GETENERGYCOUNT_F               pfncGetEnergyCount =             (GETENERGYCOUNT_F              )0x4E7750;
IMAGEFROMCANVAS_F              pfncImageFromCanvas =            (IMAGEFROMCANVAS_F             )0x619710;
GETELEVATION_F                 pfncGetElevation =               (GETELEVATION_F                )0x5919C0;
CIMAGE_COPYTOCANVAS2_F         pfncImageCopytoCanvas2 =         (CIMAGE_COPYTOCANVAS2_F        )0x6233C0;
CMAINMENU_ADDSUBMENU_F         pfncMainMenuAddSubMenu =         (CMAINMENU_ADDSUBMENU_F        )0x5FB100;
CMAINMENU_ADDBASEMENU_F        pfncMainMenuAddBaseMenu =        (CMAINMENU_ADDBASEMENU_F       )0x5FAEF0;
CMAINMENU_ADDSEPARATOR_F       pfncMainMenuAddSeparator =       (CMAINMENU_ADDSEPARATOR_F      )0x5FB160;
CMAINMENU_UPDATEVISIBLE_F      pfncMainMenuUpdateVisible =      (CMAINMENU_UPDATEVISIBLE_F     )0x460DD0;
CMAINMENU_RENAMEMENUITEM_F     pfncMainMenuRenameMenuItem =     (CMAINMENU_RENAMEMENUITEM_F    )0x5FB700;
CMAP_GETCORNERYOFFSET_F        pfncMapGetCornerYOffset =        (CMAP_GETCORNERYOFFSET_F       )0x46FE70;

// End of PRACX definitions

FMapWin_pixel_to_tile MapWin_pixel_to_tile = (FMapWin_pixel_to_tile)0x463040;
FMapWin_tile_to_pixel MapWin_tile_to_pixel = (FMapWin_tile_to_pixel)0x462F00;
FMapWin_set_center    MapWin_set_center    = (FMapWin_set_center   )0x46B1F0;
FMapWin               MapWin_calculate_dim = (FMapWin              )0x462980;
FMapWin               MapWin_clear_terrain = (FMapWin              )0x46FD90;
FWin_is_visible       Win_is_visible       = (FWin_is_visible      )0x5F7E90;
FTutWin_draw_arrow    TutWin_draw_arrow    = (FTutWin_draw_arrow   )0x4BDEA0;
FPlanWin_blink        PlanWin_blink        = (FPlanWin_blink       )0x48BC20;
FStringBox_clip_ids   StringBox_clip_ids   = (FStringBox_clip_ids  )0x629A70;
FStatusWin_on_redraw  StatusWin_on_redraw  = (FStatusWin_on_redraw )0x4B8890;
FMapWin_gen_map       MapWin_gen_map       = (FMapWin_gen_map      )0x469CA0;
FMapWin_draw_map      MapWin_draw_map      = (FMapWin_draw_map     )0x46A550;
FWinProc              WinProc              = (FWinProc             )0x5F0650;
FConsole_zoom         Console_zoom         = (FConsole_zoom        )0x5150D0;
FWin_update_screen    Win_update_screen    = (FWin_update_screen   )0x5F7320;
FWin_flip             Win_flip             = (FWin_flip            )0x5EFD20;
FBuffer_set_text_color Buffer_set_text_color = (FBuffer_set_text_color)0x5DACB0;
FBuffer_set_font       Buffer_set_font       = (FBuffer_set_font      )0x5DAC70;
FBuffer_write_cent_l3  Buffer_write_cent_l3  = (FBuffer_write_cent_l3 )0x5DD130;
Fpopup_start           popup_start           = (Fpopup_start          )0x406380;

tc_1int SubIf_release_iface_mode = (tc_1int)0x45D380;

Console* MapWin    = (Console*)0x9156B0; // ConsoleParent len: 0x247A4 end: 0x939E54
Win*     BaseWin   = (Win*)0x6A7628;
Win*     StringBox = (Win*)0x7CD2EC;
Win*     PlanWin   = (Win*)0x834D70;
Win*     SocialWin = (Win*)0x8A6270;
Win*     StatusWin = (Win*)0x8C5568;
Win*     TutWin    = (Win*)0x8C6E68;
Win*     WorldWin  = (Win*)0x8E9F60;
Win*     CouncilWin  = (Win*)0x6FEC80;
Win*     DatalinkWin = (Win*)0x703EA0;

int __thiscall SubIf_release_handler(int ptr) {
    SubIf_release_iface_mode(ptr);
    return draw_map(1);
}

bool map_is_visible() {
    return CState.TimersEnabled
        && !*GameHalted
        && !Win_is_visible(BaseWin)
        && !Win_is_visible(SocialWin)
        && !Win_is_visible(TutWin)
        && !Win_is_visible(DatalinkWin);
}

void mouse_over_tile(POINT* p) {
    static POINT ptLastTile = {0, 0};
    POINT ptTile;

    if (CState.MouseOverTileInfo
    && !pMain->fUnitNotViewMode
    && p->x >= 0 && p->x < CState.ScreenSize.x
    && p->y >= 0 && p->y < (CState.ScreenSize.y - ConsoleHeight)
    && MapWin_pixel_to_tile(pMain, p->x, p->y, &ptTile.x, &ptTile.y) == 0
    && memcmp(&ptTile, &ptLastTile, sizeof(POINT)) != 0) {

        pInfoWin->iTileX = ptTile.x;
        pInfoWin->iTileY = ptTile.y;
        StatusWin_on_redraw(pInfoWin);
        memcpy(&ptLastTile, &ptTile, sizeof(POINT));
    }
}

ULONGLONG get_ms_count() {
    static LONGLONG llFrequency = 0;
    static ULONGLONG ullLast = 0;
    static ULONGLONG ullHigh = 0;
    ULONGLONG ullRet;
    if (llFrequency == 0 && !QueryPerformanceFrequency((LARGE_INTEGER*)&llFrequency)) {
        llFrequency = -1LL;
    }
    if (llFrequency > 0) {
        QueryPerformanceCounter((LARGE_INTEGER*)&ullRet);
        ullRet *= 1000;
        ullRet /= (ULONGLONG)llFrequency;
    } else if (llFrequency < 0) {
        ullRet = GetTickCount();
        if (ullRet < ullLast) {
            ullHigh += 0x100000000ULL;
        }
        ullLast = ullRet;
        ullRet += ullHigh;
    }
    return ullRet;
}

bool do_scroll(double x, double y) {
    bool fScrolled = false;
    int mx = *map_axis_x;
    int my = *map_axis_y;
    int i;
    int d;
    if (x && pMain->oMap.iMapTilesEvenX + pMain->oMap.iMapTilesOddX < mx) {
        if (x < 0 && (!(*map_toggle_flat) || pMain->oMap.iMapTileLeft > 0)) {
            i = (int)CState.ScrollOffsetX;
            CState.ScrollOffsetX -= x;
            fScrolled = fScrolled || (i != (int)CState.ScrollOffsetX);
            while (CState.ScrollOffsetX >= pMain->oMap.iPixelsPerTileX) {
                CState.ScrollOffsetX -= pMain->oMap.iPixelsPerTileX;
                pMain->oMap.iTileX -= 2;
                if (pMain->oMap.iTileX < 0) {
                    if (*map_toggle_flat) {
                        pMain->oMap.iTileX = 0;
                        pMain->oMap.iTileY &= ~1;
                        CState.ScrollOffsetX = 0;
                    } else {
                        pMain->oMap.iTileX += mx;
                    }
                }
            }
        } else if (x < 0 && (*map_toggle_flat)) {
            fScrolled = true;
            CState.ScrollOffsetX = 0;
        }
        if (x > 0 &&
                (!(*map_toggle_flat) ||
                 pMain->oMap.iMapTileLeft +
                 pMain->oMap.iMapTilesEvenX +
                 pMain->oMap.iMapTilesOddX <= mx)) {
            i = (int)CState.ScrollOffsetX;
            CState.ScrollOffsetX -= x;
            fScrolled = fScrolled || (i != (int)CState.ScrollOffsetX);
            while (CState.ScrollOffsetX <= -pMain->oMap.iPixelsPerTileX) {
                CState.ScrollOffsetX += pMain->oMap.iPixelsPerTileX;
                pMain->oMap.iTileX += 2;
                if (pMain->oMap.iTileX > mx) {
                    if (*map_toggle_flat) {
                        pMain->oMap.iTileX = mx;
                        pMain->oMap.iTileY &= ~1;
                        CState.ScrollOffsetX = 0;
                    } else {
                        pMain->oMap.iTileX -= mx;
                    }
                }
            }
        } else if (x > 0 && (*map_toggle_flat)) {
            fScrolled = true;
            CState.ScrollOffsetX = 0;
        }
    }
    if (y && pMain->oMap.iMapTilesEvenY + pMain->oMap.iMapTilesOddY < my) {
        int iMinTileY = pMain->oMap.iMapTilesOddY - 2;
        int iMaxTileY = my + 4 - pMain->oMap.iMapTilesOddY;
        while (pMain->oMap.iTileY < iMinTileY) {
            pMain->oMap.iTileY += 2;
        }
        while (pMain->oMap.iTileY > iMaxTileY) {
            pMain->oMap.iTileY -= 2;
        }
        d = (pMain->oMap.iTileY - iMinTileY) * pMain->oMap.iPixelsPerHalfTileY - (int)CState.ScrollOffsetY;
        if (y < 0 && d > 0 ) {
            if (y < -d)
                y = -d;
            i = (int)CState.ScrollOffsetY;
            CState.ScrollOffsetY -= y;
            fScrolled = fScrolled || (i != (int)CState.ScrollOffsetY);
            while (CState.ScrollOffsetY >= pMain->oMap.iPixelsPerTileY && pMain->oMap.iTileY - 2 >= iMinTileY) {
                CState.ScrollOffsetY -= pMain->oMap.iPixelsPerTileY;
                pMain->oMap.iTileY -= 2;
            }
        }
        d = (iMaxTileY - pMain->oMap.iTileY + 1) * pMain->oMap.iPixelsPerHalfTileY + (int)CState.ScrollOffsetY;
        if (y > 0 && d > 0) {
            if (y > d)
                y = d;
            i = (int)CState.ScrollOffsetY;
            CState.ScrollOffsetY -= y;
            fScrolled = fScrolled || (i != (int)CState.ScrollOffsetY);
            while (CState.ScrollOffsetY <= -pMain->oMap.iPixelsPerTileY && pMain->oMap.iTileY + 2 <= iMaxTileY) {
                CState.ScrollOffsetY += pMain->oMap.iPixelsPerTileY;
                pMain->oMap.iTileY += 2;
            }
        }
    }
    if (fScrolled) {
        MapWin_draw_map(pMain, 0);
        Win_update_screen(NULL, 0);
        Win_flip(NULL);
        ValidateRect(*phWnd, NULL);
    }
    return fScrolled;
}

void check_scroll() {
    POINT p;
    if (CState.Scrolling || (!GetCursorPos(&p) && !(CState.ScrollDragging && CState.RightButtonDown))) {
        return;
    }
    CState.Scrolling = true;
    int w = CState.ScreenSize.x;
    int h = CState.ScreenSize.y;
    static ULONGLONG ullDeactiveTimer = 0;
    ULONGLONG ullOldTickCount = 0;
    ULONGLONG ullNewTickCount = 0;
    BOOL fScrolled;
    BOOL fScrolledAtAll = false;
    BOOL fLeftButtonDown = (GetAsyncKeyState(VK_LBUTTON) < 0);
    int iScrollArea = conf.scroll_area * CState.ScreenSize.x / 1024;

    if (CState.RightButtonDown && GetAsyncKeyState(VK_RBUTTON) < 0) {
        if (labs((long)hypot((double)(p.x-CState.ScrollDragPos.x), (double)(p.y-CState.ScrollDragPos.y))) > 2.5) {
            CState.ScrollDragging = true;
            SetCursor(LoadCursor(0, IDC_HAND));
        }
    }
    CState.ScrollOffsetX = pMain->oMap.iMapPixelLeft;
    CState.ScrollOffsetY = pMain->oMap.iMapPixelTop;
    ullNewTickCount = get_ms_count();
    ullOldTickCount = ullNewTickCount;
//    debug("scroll_check %d %d %d\n", CState.Scrolling, (int)CState.ScrollDragPos.x, (int)CState.ScrollDragPos.y);
    do {
        double dTPS = -1;
        double dx = 0;
        double dy = 0;
        fScrolled = false;
        if (CState.ScrollDragging && CState.RightButtonDown) {
            fScrolled = true;
            dx = CState.ScrollDragPos.x - p.x;
            dy = CState.ScrollDragPos.y - p.y;
            memcpy(&CState.ScrollDragPos, &p, sizeof(POINT));

        } else if (ullNewTickCount - ullDeactiveTimer > 100 && !CState.ScrollDragging) {
            double dMin = (double)CState.ScrollMin;
            double dMax = (double)CState.ScrollMax;
            double dArea = (double)iScrollArea;
            if (p.x <= iScrollArea && p.x >= 0) {
                fScrolled = true;
                dTPS = dMin + (dArea - (double)p.x) / dArea * (dMax - dMin);
                dx = (double)(ullNewTickCount - ullOldTickCount) * dTPS * (double)pMain->oMap.iPixelsPerTileX / -1000.0;

            } else if ((w - p.x) <= iScrollArea && w >= p.x) {
                fScrolled = true;
                dTPS = dMin + (dArea - (double)(w - p.x)) / dArea * (dMax - dMin);
                dx = (double)(ullNewTickCount - ullOldTickCount) * dTPS * (double)pMain->oMap.iPixelsPerTileX / 1000.0;
            }
            if (p.y <= iScrollArea && p.y >= 0) {
                fScrolled = true;
                dTPS = dMin + (dArea - (double)p.y) / dArea * (dMax - dMin);
                dy = (double)(ullNewTickCount - ullOldTickCount) * dTPS * (double)pMain->oMap.iPixelsPerTileY / -1000.0;

            } else if (h - p.y <= iScrollArea && h >= p.y &&
            // These extra conditions will stop movement when the mouse is over the bottom middle console.
            (p.x <= (CState.ScreenSize.x - ConsoleWidth) / 2 ||
             p.x >= (CState.ScreenSize.x - ConsoleWidth) / 2 + ConsoleWidth ||
             h - p.y <= 8 * CState.ScreenSize.y / 768)) {
                fScrolled = true;
                dTPS = dMin + (dArea - (double)(h - p.y)) / dArea * (dMax - dMin);
                dy = (double)(ullNewTickCount - ullOldTickCount) * dTPS * (double)pMain->oMap.iPixelsPerTileY / 1000.0;
            }
        }
        if (fScrolled) {
            ullOldTickCount = ullNewTickCount;
            if (do_scroll(dx, dy)) {
                fScrolledAtAll = true;
            } else {
                Sleep(0);
            }
            if (DEBUG) {
                Sleep(5);
            }
            ullNewTickCount = get_ms_count();
            if (CState.RightButtonDown) {
                CState.RightButtonDown = (GetAsyncKeyState(VK_RBUTTON) < 0);
            }
            debug("scroll_move  x=%d y=%d scx=%.4f scy=%.4f dx=%.4f dy=%.4f dTPS=%.4f\n",
                (int)p.x, (int)p.y, CState.ScrollOffsetX, CState.ScrollOffsetY, dx, dy, dTPS);
        }

    } while (fScrolled && (GetCursorPos(&p) || (CState.ScrollDragging && CState.RightButtonDown)));

    // TODO: determine the purpose of this code
    if (fScrolledAtAll) {
        pMain->oMap.drawOnlyCursor = 1;
        MapWin_set_center(pMain, pMain->oMap.iTileX, pMain->oMap.iTileY, 1);
        pMain->oMap.drawOnlyCursor = 0;
        for (int i = 1; i < 8; i++) {
            if (ppMain[i] && ppMain[i]->oMap.field_1DD74 &&
            (!fLeftButtonDown || ppMain[i]->oMap.field_1DD80) &&
            ppMain[i]->oMap.iMapTilesOddX + ppMain[i]->oMap.iMapTilesEvenX < *map_axis_x) {
                MapWin_set_center(ppMain[i], pMain->oMap.iTileX, pMain->oMap.iTileY, 1);
            }
        }
        if (CState.ScrollDragging) {
            ullDeactiveTimer = ullNewTickCount;
        }
    }
    mouse_over_tile(&p);
    CState.Scrolling = false;
    flushlog();
}

LRESULT WINAPI ModWinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static int iDeltaAccum = 0;
    bool fHasFocus = (GetFocus() == *phWnd);
    bool tools = DEBUG && !*GameHalted;
    POINT p;

    if (msg == WM_MOUSEWHEEL && fHasFocus) {
        int iDelta = GET_WHEEL_DELTA_WPARAM(wParam) + iDeltaAccum;
        iDeltaAccum = iDelta % WHEEL_DELTA;
        iDelta /= WHEEL_DELTA;
        bool zoom_in = (iDelta >= 0);
        iDelta = labs(iDelta);

        if (map_is_visible() && *map_axis_x) {
            int iZoomType = (zoom_in ? 515 : 516);
            for (int i = 0; i < iDelta; i++) {
                if (pMain->oMap.iZoomFactor > -8 || zoom_in) {
                    Console_zoom(iZoomType, 0);
                }
            }
        } else {
            int iKey = (zoom_in ? VK_UP : VK_DOWN);
            iDelta *= CState.ListScrollDelta;
            for (int i = 0; i < iDelta; i++) {
                PostMessage(hwnd, WM_KEYDOWN, iKey, 0);
                PostMessage(hwnd, WM_KEYUP, iKey, 0);
            }
            return 0;
        }

    } else if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST) {
        if (!map_is_visible() || !fHasFocus) {
            CState.RightButtonDown = false;
            CState.ScrollDragging = false;
            return WinProc(hwnd, msg, wParam, lParam);

        } else if (msg == WM_RBUTTONDOWN) {
            CState.RightButtonDown = true;
            GetCursorPos(&p);
            memcpy(&CState.ScrollDragPos, &p, sizeof(POINT));

        } else if (msg == WM_RBUTTONUP) {
            CState.RightButtonDown = false;
            if (CState.ScrollDragging) {
                CState.ScrollDragging = false;
                SetCursor(LoadCursor(0, IDC_ARROW));
            } else {
                WinProc(hwnd, WM_RBUTTONDOWN, wParam | MK_RBUTTON, lParam);
                return WinProc(hwnd, WM_RBUTTONUP, wParam, lParam);
            }
        } else if (CState.RightButtonDown) {
            check_scroll();
        } else {
            return WinProc(hwnd, msg, wParam, lParam);
        }
    } else if (msg == WM_ACTIVATEAPP) {
        // If window has just become inactive e.g. ALT+TAB
        if (LOWORD(wParam)) {
            ShowWindow(*phWnd, SW_RESTORE);
        } else {
            //wParam is 0 if the window has become inactive.
            //ShowWindow(*phWnd, SW_MINIMIZE);
        }
        return WinProc(hwnd, msg, wParam, lParam);

    } else if (msg == WM_CHAR && wParam == 'r' && GetAsyncKeyState(VK_MENU) < 0) {
        CState.MouseOverTileInfo = !CState.MouseOverTileInfo;

    } else if (msg == WM_CHAR && wParam == 't' && GetAsyncKeyState(VK_MENU) < 0) {
        show_mod_menu();

    } else if (DEBUG && msg == WM_CHAR && wParam == 'd' && GetAsyncKeyState(VK_MENU) < 0) {
        conf.debug_mode = !conf.debug_mode;
        if (conf.debug_mode) {
            for (int i = 1; i < MaxPlayerNum; i++) {
                Faction& f = Factions[i];
                if (!f.base_count) {
                    memset(f.goals, 0, sizeof(f.goals));
                    memset(f.sites, 0, sizeof(f.sites));
                }
            }
            *game_state |= STATE_DEBUG_MODE;
            *game_preferences |= PREF_ADV_FAST_BATTLE_RESOLUTION;
            *game_more_preferences |=
                (MPREF_ADV_QUICK_MOVE_VEH_ORDERS | MPREF_ADV_QUICK_MOVE_ALL_VEH);
        } else {
            *game_state &= ~STATE_DEBUG_MODE;
        }
        if (!*GameHalted) {
            MapWin_draw_map(pMain, 0);
            InvalidateRect(hwnd, NULL, false);
        }

    } else if (DEBUG && msg == WM_CHAR && wParam == 'm' && GetAsyncKeyState(VK_MENU) < 0) {
        conf.debug_verbose = !conf.debug_verbose;
        parse_says(0, MOD_VERSION, -1, -1);
        parse_says(1, (conf.debug_verbose ?
            "Verbose mode enabled." : "Verbose mode disabled."), -1, -1);
        popp("modmenu", "GENERIC", 0, 0, 0);

    } else if (tools && msg == WM_CHAR && wParam == 'y' && GetAsyncKeyState(VK_MENU) < 0) {
        static int draw_diplo = 0;
        draw_diplo = !draw_diplo;
        if (draw_diplo) {
            pMain->oMap.iWhatToDrawFlags |= MAPWIN_DRAW_DIPLO_STATE;
            *game_state |= STATE_DEBUG_MODE;
        } else {
            pMain->oMap.iWhatToDrawFlags &= ~MAPWIN_DRAW_DIPLO_STATE;
        }
        MapWin_draw_map(pMain, 0);
        InvalidateRect(hwnd, NULL, false);

    } else if (tools && msg == WM_CHAR && wParam == 'v' && GetAsyncKeyState(VK_MENU) < 0) {
        pMain->oMap.iWhatToDrawFlags |= MAPWIN_DRAW_GOALS;
        memset(pm_overlay, 0, sizeof(pm_overlay));
        static int ts_type = 0;
        int i = 0;
        TileSearch ts;
        ts_type = (ts_type+1) % (MaxTileSearchType+1);
        ts.init(pMain->oMap.iTileX, pMain->oMap.iTileY, ts_type, 0);
        while (ts.get_next() != NULL) {
            pm_overlay[ts.rx][ts.ry] = ++i;
        }
        pm_overlay[pMain->oMap.iTileX][pMain->oMap.iTileY] = ts_type;
        MapWin_draw_map(pMain, 0);
        InvalidateRect(hwnd, NULL, false);

    } else if (tools && msg == WM_CHAR && wParam == 'f' && GetAsyncKeyState(VK_MENU) < 0) {
        pMain->oMap.iWhatToDrawFlags |= MAPWIN_DRAW_GOALS;
        memset(pm_overlay, 0, sizeof(pm_overlay));
        MAP* sq = mapsq(pMain->oMap.iTileX, pMain->oMap.iTileY);
        if (sq && sq->is_owned()) {
            move_upkeep(sq->owner, M_Visual);
            MapWin_draw_map(pMain, 0);
            InvalidateRect(hwnd, NULL, false);
        }

    } else if (tools && msg == WM_CHAR && wParam == 'x' && GetAsyncKeyState(VK_MENU) < 0) {
        pMain->oMap.iWhatToDrawFlags |= MAPWIN_DRAW_GOALS;
        static int px = 0, py = 0;
        int x = pMain->oMap.iTileX, y = pMain->oMap.iTileY;
        int unit = is_ocean(mapsq(x, y)) ? BSC_UNITY_FOIL : BSC_UNITY_ROVER;
        path_distance(px, py, x, y, unit, 1);
        px=x;
        py=y;
        MapWin_draw_map(pMain, 0);
        InvalidateRect(hwnd, NULL, false);

    } else if (tools && msg == WM_CHAR && wParam == 'z' && GetAsyncKeyState(VK_MENU) < 0) {
        int x = pMain->oMap.iTileX, y = pMain->oMap.iTileY;
        int base_id;
        if ((base_id = base_at(x, y)) >= 0) {
            print_base(base_id);
        }
        print_map(x, y);
        for (int k=0; k < *total_num_vehicles; k++) {
            VEH* veh = &Vehicles[k];
            if (veh->x == x && veh->y == y) {
                Vehicles[k].state |= VSTATE_UNK_40000;
                Vehicles[k].state &= ~VSTATE_UNK_2000;
                print_veh(k);
            }
        }
        flushlog();

    } else {
        return WinProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/*
Render custom debug overlays with original goals.
*/
void __thiscall MapWin_gen_overlays(Console* This, int x, int y)
{
    Buffer* Canvas = (Buffer*)0x939888;
    RECT rt;
    if (*game_state & STATE_OMNISCIENT_VIEW && pMain->oMap.iWhatToDrawFlags & MAPWIN_DRAW_GOALS)
    {
        MapWin_tile_to_pixel(This, x, y, &rt.left, &rt.top);
        rt.right = rt.left + This->oMap.iPixelsPerTileX;
        rt.bottom = rt.top + This->oMap.iPixelsPerHalfTileX;

        char buf[20] = {};
        bool found = false;
        int color = 255;
        int value = pm_overlay[x][y];

        for (int faction = 1; faction < MaxPlayerNum && !found; faction++) {
            Faction& f = Factions[faction];
            MFaction& m = MFactions[faction];
            if (!f.base_count) {
                continue;
            }
            for (int i = 0; i < MaxGoalsNum && !found; i++) {
                Goal& goal = Factions[faction].goals[i];
                if (goal.x == x && goal.y == y && goal.priority > 0
                && goal.type != AI_GOAL_UNUSED ) {
                    found = true;
                    buf[0] = m.filename[0];
                    switch (goal.type) {
                        case AI_GOAL_ATTACK:
                            buf[1] = 'a';
                            color = 249;
                            break;
                        case AI_GOAL_DEFEND:
                            buf[1] = 'd';
                            color = 251;
                            break;
                        case AI_GOAL_SCOUT:
                            buf[1] = 's';
                            color = 253;
                            break;
                        case AI_GOAL_UNK_1:
                            buf[1] = 'n';
                            color = 252;
                            break;
                        case AI_GOAL_COLONIZE:
                            buf[1] = 'c';
                            color = 254;
                            break;
                        case AI_GOAL_TERRAFORM_LAND:
                            buf[1] = 'f';
                            color = 250;
                            break;
                        case AI_GOAL_UNK_4:
                            buf[1] = '^';
                            break;
                        case AI_GOAL_RAISE_LAND:
                            buf[1] = 'r';
                            break;
                        default:
                            buf[1] = (goal.type < 200 ? '*' : 'g');
                            break;
                    }
                    _itoa(goal.priority, &buf[2], 10);
                }
            }
        }
        if (!found && value != 0) {
            color = (value >= 0 ? 255 : 251);
            _itoa(value, buf, 10);
        }
        if (found || value) {
            Buffer_set_text_color(Canvas, color, 0, 1, 1);
            Buffer_set_font(Canvas, &This->oMap.oFont2, 0, 0, 0);
            Buffer_write_cent_l3(Canvas, buf, &rt, 10);
        }
    }
}

/*
Override Windows API call to give fake screensize values to SMACX while in windowed mode.
When DirectDraw=0 is set, this allows us to force SMACX run in a borderless window, enabling
very fast rendering and full user access to the other windows.
*/
int WINAPI ModGetSystemMetrics(int nIndex) {
    if (conf.windowed) {
        if (nIndex == SM_CXSCREEN) {
            return conf.window_width;
        }
        if (nIndex == SM_CYSCREEN) {
            return conf.window_height;
        }
    }
    return GetSystemMetrics(nIndex);
}

ATOM WINAPI ModRegisterClassA(WNDCLASS* pstWndClass) {
    pstWndClass->lpfnWndProc = ModWinProc;
    return RegisterClassA(pstWndClass);
}

int __thiscall mod_gen_map(Console* This, int iOwner, int fUnitsOnly) {

    if (This == pMain) {
        debug("mod_gen_map %d %.4f %.4f\n", CState.Scrolling, CState.ScrollOffsetX, CState.ScrollOffsetY);

        // Save these values to restore them later
        int iMapPixelLeft = This->oMap.iMapPixelLeft;
        int iMapPixelTop = This->oMap.iMapPixelTop;
        int iMapTileLeft = This->oMap.iMapTileLeft;
        int iMapTileTop = This->oMap.iMapTileTop;
        int iMapTilesOddX = This->oMap.iMapTilesOddX;
        int iMapTilesOddY = This->oMap.iMapTilesOddY;
        int iMapTilesEvenX = This->oMap.iMapTilesEvenX;
        int iMapTilesEvenY = This->oMap.iMapTilesEvenY;
        // These are just aliased to save typing and are not modified
        int mx = *map_axis_x;
        int my = *map_axis_y;

        if (iMapTilesOddX + iMapTilesEvenX < mx && !(*map_toggle_flat)) {
            if (iMapPixelLeft > 0) {
                This->oMap.iMapPixelLeft -= This->oMap.iPixelsPerTileX;
                This->oMap.iMapTileLeft -= 2;
                This->oMap.iMapTilesEvenX++;
                This->oMap.iMapTilesOddX++;
                if (This->oMap.iMapTileLeft < 0)
                    This->oMap.iMapTileLeft += mx;
            } else if (iMapPixelLeft < 0 ) {
                This->oMap.iMapTilesEvenX++;
                This->oMap.iMapTilesOddX++;
            }
        }
        if (iMapTilesOddY + iMapTilesEvenY < my) {
            if (iMapPixelTop > 0) {
                This->oMap.iMapPixelTop -= This->oMap.iPixelsPerTileY;
                This->oMap.iMapTileTop -= 2;
                This->oMap.iMapTilesEvenY++;
                This->oMap.iMapTilesOddY++;
            } else if (iMapPixelTop < 0) {
                This->oMap.iMapTilesEvenY++;
                This->oMap.iMapTilesOddY++;
            }
        }
        MapWin_gen_map(This, iOwner, fUnitsOnly);
        // Restore This's original values
        This->oMap.iMapPixelLeft = iMapPixelLeft;
        This->oMap.iMapPixelTop = iMapPixelTop;
        This->oMap.iMapTileLeft = iMapTileLeft;
        This->oMap.iMapTileTop = iMapTileTop;
        This->oMap.iMapTilesOddX = iMapTilesOddX;
        This->oMap.iMapTilesOddY = iMapTilesOddY;
        This->oMap.iMapTilesEvenX = iMapTilesEvenX;
        This->oMap.iMapTilesEvenY = iMapTilesEvenY;
    } else {
        MapWin_gen_map(This, iOwner, fUnitsOnly);
    }
    return 0;
}

int __thiscall mod_calc_dim(Console* This) {
    static POINT ptOldTile = {-1, -1};
    POINT ptOldCenter;
    POINT ptNewCenter;
    POINT ptNewTile;
    POINT ptScale;
    int iOldZoom;
    int dx, dy;
    bool fx, fy;
//    int w = ((CWinBuffed*)((int)This + (int)This->oMap.vtbl->iOffsetofoClass2))->oCanvas.stBitMapInfo.bmiHeader.biWidth;
//    int h = -((CWinBuffed*)((int)This + (int)This->oMap.vtbl->iOffsetofoClass2))->oCanvas.stBitMapInfo.bmiHeader.biHeight;

    if (This == pMain) {
        debug("mod_calc_dim %d %.4f %.4f\n",  CState.Scrolling, CState.ScrollOffsetX, CState.ScrollOffsetY);
        iOldZoom = This->oMap.iLastZoomFactor;
        ptNewTile.x = This->oMap.iTileX;
        ptNewTile.y = This->oMap.iTileY;
        fx = (ptNewTile.x == ptOldTile.x);
        fy = (ptNewTile.y == ptOldTile.y);
        memcpy(&ptOldTile, &ptNewTile, sizeof(POINT));
        MapWin_tile_to_pixel(This, ptNewTile.x, ptNewTile.y, &ptOldCenter.x, &ptOldCenter.y);
        MapWin_calculate_dim(This);

        if (CState.Scrolling) {
            This->oMap.iMapPixelLeft = (int)CState.ScrollOffsetX;
            This->oMap.iMapPixelTop = (int)CState.ScrollOffsetY;
        } else if (iOldZoom != -9999) {
            ptScale.x = This->oMap.iPixelsPerTileX;
            ptScale.y = This->oMap.iPixelsPerTileY;
            MapWin_tile_to_pixel(This, ptNewTile.x, ptNewTile.y, &ptNewCenter.x, &ptNewCenter.y);
            dx = ptOldCenter.x - ptNewCenter.x;
            dy = ptOldCenter.y - ptNewCenter.y;
            if (!This->oMap.iMapPixelLeft && fx && dx > -ptScale.x * 2 && dx < ptScale.x * 2) {
                This->oMap.iMapPixelLeft = dx;
            }
            if (!This->oMap.iMapPixelTop && fy && dy > -ptScale.y * 2 && dy < ptScale.y * 2
            && (dy + *screen_height) / ptScale.y < (*map_axis_y - This->oMap.iMapTileTop) / 2) {
                This->oMap.iMapPixelTop = dy;
            }
        }
    } else {
        MapWin_calculate_dim(This);
    }
    return 0;
}

/*
Original Offset: 0050EA40
*/
int __cdecl blink_timer() {
    if (!*GameHalted && !*ControlRedraw) {
        if (Win_is_visible(BaseWin)) {
            return TutWin_draw_arrow(TutWin);
        }
        if (Win_is_visible(SocialWin)) {
            return TutWin_draw_arrow(TutWin);
        }
        PlanWin_blink(PlanWin);
        StringBox_clip_ids(StringBox, 150);

        if ((!MapWin->field_23BE8 && (!*multiplayer_active || !(*game_state & STATE_UNK_2))) || *ControlTurnA) {
            MapWin->field_23BFC = 0;
            return 0;
        }
        if (MapWin->field_23BE4 || (*multiplayer_active && *game_state & STATE_UNK_2)) {
            MapWin->field_23BF8 = !MapWin->field_23BF8;
            draw_cursor();
            bool mouse_btn_down = GetAsyncKeyState(VK_LBUTTON) < 0;

            if ((int)*phInstance == GetWindowLongA(GetFocus(), GWL_HINSTANCE) && !Win_is_visible(TutWin)) {
                if (MapWin->field_23D80 != -1
                && MapWin->field_23D84 != -1
                && MapWin->field_23D88 != -1
                && MapWin->field_23D8C != -1) {
                    if (*game_preferences & PREF_BSC_MOUSE_EDGE_SCROLL_VIEW || mouse_btn_down || *dword_9B7AE4 != 0) {
                        CState.ScreenSize.x = *screen_width;
                        CState.ScreenSize.y = *screen_height;
                        CState.TimersEnabled = true;
                        check_scroll();
                    }
                }
            }
        }
    }
    return 0;
}

void __cdecl mod_turn_timer() {
    // Timer calls function every 500ms
    static uint32_t iter = 0;
    static uint32_t prev_time = 0;
    static int32_t prev_seed = 0;
    turn_timer();
    if (++iter & 1) {
        return;
    }
    if (*GameHalted && !*map_random_seed) {
        ThinkerVars->game_time_spent = 0;
        prev_time = 0;
    } else {
        uint32_t now = GetTickCount();
        if (prev_time && now - prev_time > 0) {
            ThinkerVars->game_time_spent += now - prev_time;
        }
        prev_time = now;
        /*
        Fix faction graphics bug that appears when Alpha Centauri.ini
        has a different set of faction filenames than the loaded scenario file.
        If the game has an incorrect faction set, it requires loading the save once,
        then quitting to main menu, and then loading the save again.
        */
        if (*game_state & STATE_IS_SCENARIO && *map_random_seed != prev_seed) {
            char key[256];
            char buf[256];
            bool restart = false;
            prev_seed = *map_random_seed;

            for (int i=1; i < MaxPlayerNum; i++) {
                snprintf(key, sizeof(key), "Faction %d", i);
                int ret = GetPrivateProfileStringA(
                    GameAppName, key, "", buf, sizeof(buf), GameIniFile);
                if (!ret || strcmp(MFactions[i].filename, buf)) {
                    restart = true;
                    WritePrivateProfileStringA(
                        GameAppName, key, MFactions[i].filename, GameIniFile);
                }
            }
            if (restart) { // Return to main menu
                *ControlTurnA = 1;
                *ControlTurnB = 1;
            }
        }
    }
}

void popup_homepage() {
    ShellExecute(NULL, "open", "https://github.com/induktio/thinker", NULL, NULL, SW_SHOWNORMAL);
}

void show_mod_stats() {
    int total_pop = 0,
        total_minerals = 0,
        total_energy = 0,
        faction_pop = 0,
        faction_units = 0,
        faction_minerals = 0,
        faction_energy = 0;

    Faction* f = &Factions[*current_player_faction];
    for (int i=0; i<*total_num_bases; ++i) {
        BASE* b = &Bases[i];
        if (b->faction_id == *current_player_faction) {
            faction_pop += b->pop_size;
            faction_minerals += b->mineral_intake_2;
            faction_energy += b->energy_intake_2;
        }
        total_pop += b->pop_size;
        total_minerals += b->mineral_intake_2;
        total_energy += b->energy_intake_2;
    }
    for (int i=0; i<*total_num_vehicles; i++) {
        VEH* v = &Vehicles[i];
        if (v->faction_id == *current_player_faction) {
            faction_units++;
        }
    }
    ParseNumTable[0] = *total_num_bases;
    ParseNumTable[1] = *total_num_vehicles;
    ParseNumTable[2] = total_pop;
    ParseNumTable[3] = total_minerals;
    ParseNumTable[4] = total_energy;
    ParseNumTable[5] = f->base_count;
    ParseNumTable[6] = faction_units;
    ParseNumTable[7] = f->pop_total;
    ParseNumTable[8] = faction_minerals;
    ParseNumTable[9] = faction_energy;
    popp("modmenu", "STATS", 0, "markbm_sm.pcx", 0);
}

int show_mod_config() {
    enum {Close, MapGen, MapStyle, MapLandmarks, MapLabels, AutoUnits, FormerReplace};
    int ret;
    while (true) {
        parse_says(0, (conf.new_world_builder ? "true" : "false"), -1, -1);
        parse_says(1, (conf.world_continents ? "large islands" : "small islands"), -1, -1);
        parse_says(2, (conf.modified_landmarks ? "true" : "false"), -1, -1);
        parse_says(3, (conf.world_map_labels ? "true" : "false"), -1, -1);
        parse_says(4, (conf.manage_player_units ? "true" : "false"), -1, -1);
        parse_says(5, (conf.warn_on_former_replace ? "true" : "false"), -1, -1);
        ret = popp("modmenu", "OPTIONS", 0, "stars_sm.pcx", 0);
        if (ret == Close) {
            break;
        }
        else if (ret == MapGen) {
            conf.new_world_builder = !conf.new_world_builder;
            WritePrivateProfileStringA(ModAppName, "new_world_builder",
                (conf.new_world_builder ? "1" : "0"), GameIniFile);
        }
        else if (ret == MapStyle) {
            conf.world_continents = !conf.world_continents;
            WritePrivateProfileStringA(ModAppName, "world_continents",
                (conf.world_continents ? "1" : "0"), GameIniFile);
        }
        else if (ret == MapLandmarks) {
            conf.modified_landmarks = !conf.modified_landmarks;
            WritePrivateProfileStringA(ModAppName, "modified_landmarks",
                (conf.modified_landmarks ? "1" : "0"), GameIniFile);
        }
        else if (ret == MapLabels) {
            conf.world_map_labels = !conf.world_map_labels;
            WritePrivateProfileStringA(ModAppName, "world_map_labels",
                (conf.world_map_labels ? "1" : "0"), GameIniFile);
            draw_map(1);
        }
        else if (ret == AutoUnits) {
            conf.manage_player_units = !conf.manage_player_units;
            WritePrivateProfileStringA(ModAppName, "manage_player_units",
                (conf.manage_player_units ? "1" : "0"), GameIniFile);
        }
        else if (ret == FormerReplace) {
            conf.warn_on_former_replace = !conf.warn_on_former_replace;
            WritePrivateProfileStringA(ModAppName, "warn_on_former_replace",
                (conf.warn_on_former_replace ? "1" : "0"), GameIniFile);
        }
    }
    return 0;
}

int show_mod_menu() {
    parse_says(0, MOD_VERSION, -1, -1);
    parse_says(1, MOD_VERSION, -1, -1);
    parse_says(2, MOD_DATE, -1, -1);

    if (*GameHalted) {
        int ret = popp("modmenu", "MAINMENU", 0, "stars_sm.pcx", 0);
        if (ret == 1) {
            popup_homepage();
        }
        return 0;
    }
    uint64_t seconds = ThinkerVars->game_time_spent / 1000;
    ParseNumTable[0] = seconds / 3600;
    ParseNumTable[1] = (seconds / 60) % 60;
    ParseNumTable[2] = seconds % 60;
    int ret = popp("modmenu", "GAMEMENU", 0, "stars_sm.pcx", 0);

    if (ret == 1 && !*GameHalted && !*pbem_active && !*multiplayer_active) {
        show_mod_stats();
    }
    else if (ret == 2 && !*GameHalted) {
        show_mod_config();
    }
    else if (ret == 3) {
        popup_homepage();
    }
    return 0;
}

static int minimal_cost = 0;

int __thiscall basewin_popup_start(Win* This,
const char* UNUSED(filename), const char* UNUSED(label), int a4, int a5, int a6, int a7)
{
    BASE* base = *current_base_ptr;
    Faction* f = &Factions[base->faction_id];
    int mins = mineral_cost(*current_base_id, base->queue_items[0])
        - base->minerals_accumulated - base->mineral_surplus;
    minimal_cost = hurry_cost(*current_base_id, base->queue_items[0], mins);
    ParseNumTable[1] = minimal_cost;
    ParseNumTable[2] = f->energy_credits - f->energy_cost;
    return popup_start(This, "modmenu", "HURRY", a4, a5, a6, a7);
}

int __cdecl basewin_ask_number(const char* label, int value, int a3)
{
    ParseNumTable[0] = value;
    return pop_ask_number("SCRIPT", label, minimal_cost, a3);
}

#pragma GCC diagnostic pop

