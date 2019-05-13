/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2000 Grace Development Team
 * 
 * Maintained by Evgeny Stambulchik
 * 
 * Modified by Andreas Winter 2008-2017
 * 
 *                           All Rights Reserved
 * 
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* 
 *
 * Read/write a parameter file
 *
 */

///#include <config.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "draw.h"
#include "globals.h"
#include "utils.h"
#include "graphs.h"
#include "graphutils.h"
#include "device.h"
#include "noxprotos.h"
#include "cmath.h"

#ifdef __cplusplus
extern "C" {
#endif
extern void prepare_strings_for_saving(void);
extern void resume_strings_after_load_or_save(void);
#ifdef __cplusplus
}
#endif

static void put_regions(FILE * pp, int embed);
static void put_objects(int gno, FILE * pp, int embed);

static void put_beast_regions(char *pp, int embed);
static void put_beast_objects(int gno, char *pp, int embed);

static char buf[256];

void putparmbeast(int gno, char *pp, int embed)
{
    unsigned int i;
    int j, k, ming, maxg;
    int ps, pt, gh, gt, fx, fy, px, py;
    double dsx, dsy;
    char embedstr[2], tmpstr1[64], tmpstr2[64];
    framep f;
    legend leg;
    labels lab;
    plotarr p;
    tickmarks *t;
    world_stack ws;
    world w;
    view v;
    CMap_entry *cmap;
    GLocator locator;
    char *p1, *p2, *tmpbuf;
    int wpp, hpp;
    (void)gno;

    if (embed) {
        strcpy(embedstr, "@");
    } else {
        embedstr[0] = 0;
    }
    sprintf(pp, "# Grace project file\n");
    sprintf(pp+strlen(pp), "#\n");
    // or
    // int pos=sprintf(pp, "# Grace project file\n");
    // pp+=pos;
    // pos=sprintf(pp, "#\n");
    // pp+=pos;
    // pos=sprintf(pp, "#\n");

    /* Print some global variables */
    sprintf(pp+strlen(pp), "%sversion %ld\n", embedstr, bi_version_id());

    get_device_page_dimensions(tdevice, &wpp, &hpp);
    sprintf(pp+strlen(pp), "%spage size %d, %d\n", embedstr, wpp, hpp);

    tmpbuf = copy_string(NULL, get_project_description());
    if (tmpbuf != NULL) {
        p1 = tmpbuf;
        while ((p2 = strchr (p1, '\n')) != NULL) {
            *p2 = 0;
            sprintf (pp, "%sdescription \"%s\"\n", embedstr, PSTRING(p1));
            *p2 = '\n';
            p1 = p2;
            p1++;
        }
        if (*p1) {
            sprintf (pp, "%sdescription \"%s\"\n", embedstr, PSTRING(p1));
        }
        xfree(tmpbuf);
    }

    sprintf(pp+strlen(pp), "%spage scroll %d%%\n", embedstr, (int) rint(scrollper * 100));
    sprintf(pp+strlen(pp), "%spage inout %d%%\n", embedstr, (int) rint(shexper * 100));
    sprintf(pp+strlen(pp), "%slink page %s\n", embedstr, scrolling_islinked ? "on" : "off");

    for (i = 0; i < number_of_fonts(); i++) {
        if (get_font_mapped_id(i) != BAD_FONT_ID) {
            sprintf(pp+strlen(pp), "%smap font %d to \"%s\", \"%s\"\n", embedstr,
                                                    get_font_mapped_id(i),
                                                    get_fontalias(i),
                                                    get_fontfallback(i));
        }
    }

    for (i = 0; i < number_of_colors(); i++) {
        cmap = get_cmap_entry(i);
        if (cmap != NULL && cmap->ctype == COLOR_MAIN) {
            sprintf(pp+strlen(pp), "%smap color %d to (%d, %d, %d), \"%s\"\n", embedstr,
                i, cmap->rgb.red, cmap->rgb.green, cmap->rgb.blue,
                PSTRING(cmap->cname));
        }
    }

    sprintf(pp+strlen(pp), "%sreference date %.16g\n", embedstr, get_ref_date());
    sprintf(pp+strlen(pp), "%sdate wrap %s\n", embedstr,
        on_or_off(two_digits_years_allowed()));
    sprintf(pp+strlen(pp), "%sdate wrap year %d\n", embedstr, get_wrap_year());

    sprintf(pp+strlen(pp), "%sdefault linewidth %.1f\n", embedstr, grdefaults.linew);
    sprintf(pp+strlen(pp), "%sdefault linestyle %d\n", embedstr, grdefaults.lines);
    sprintf(pp+strlen(pp), "%sdefault color %d\n", embedstr, grdefaults.color);
    sprintf(pp+strlen(pp), "%sdefault pattern %d\n", embedstr, grdefaults.pattern);
    sprintf(pp+strlen(pp), "%sdefault font %d\n", embedstr, get_font_mapped_id(grdefaults.font));
    sprintf(pp+strlen(pp), "%sdefault char size %f\n", embedstr, grdefaults.charsize);
    sprintf(pp+strlen(pp), "%sdefault symbol size %f\n", embedstr, grdefaults.symsize);
    sprintf(pp+strlen(pp), "%sdefault sformat \"%s\"\n", embedstr, PSTRING(sformat));

    sprintf(pp+strlen(pp), "%sbackground color %d\n", embedstr, getbgcolor());
    sprintf(pp+strlen(pp), "%spage background fill %s\n", embedstr, on_or_off(getbgfill()));

/*
    sprintf(pp+strlen(pp), "%stimestamp %s\n", embedstr, on_or_off(timestamp.active));
    sprintf(pp+strlen(pp), "%stimestamp %.16g, %.16g\n", embedstr, timestamp.x, timestamp.y);
    sprintf(pp+strlen(pp), "%stimestamp color %d\n", embedstr, timestamp.color);
    sprintf(pp+strlen(pp), "%stimestamp rot %d\n", embedstr, timestamp.rot);
    sprintf(pp+strlen(pp), "%stimestamp font %d\n", embedstr, get_font_mapped_id(timestamp.font));
    sprintf(pp+strlen(pp), "%stimestamp char size %f\n", embedstr, timestamp.charsize);
    sprintf(pp+strlen(pp), "%stimestamp def \"%s\"\n", embedstr, PSTRING(timestamp.s_plotstring));
*/

    put_beast_objects(gno, pp, embed);

    put_beast_regions(pp, embed);

    if (gno == ALL_GRAPHS) {
        maxg = number_of_graphs() - 1;
        ming = 0;
    } else {
        maxg = gno;
        ming = gno;
    }
    for (k = ming; k <= maxg; k++) {
        if (is_graph_active(k)) {
            gno = k;
            gh = is_graph_hidden(gno);
            gt = get_graph_type(gno);
            get_graph_locator(gno, &locator);
            ps = locator.pointset;
            pt = locator.pt_type;
            dsx = locator.dsx;
            dsy = locator.dsy;
            fx = locator.fx;
            fy = locator.fy;
            px = locator.px;
            py = locator.py;

            sprintf(pp+strlen(pp), "%sg%1d %s\n", embedstr, gno,
                                            on_or_off(is_graph_active(gno)));
            sprintf(pp+strlen(pp), "%sg%1d hidden %s\n", embedstr, gno,
                                            true_or_false(gh));
            sprintf(pp+strlen(pp), "%sg%1d type %s\n", embedstr, gno, graph_types(gt));
            sprintf(pp+strlen(pp), "%sg%1d stacked %s\n", embedstr, gno,
                                        true_or_false(is_graph_stacked(gno)));

            sprintf(pp+strlen(pp), "%sg%1d bar hgap %f\n", embedstr, gno,
                                                        get_graph_bargap(gno));

            sprintf(pp+strlen(pp), "%sg%1d fixedpoint %s\n", embedstr, gno, on_or_off(ps));
            sprintf(pp+strlen(pp), "%sg%1d fixedpoint type %d\n", embedstr, gno, pt);
            sprintf(pp+strlen(pp), "%sg%1d fixedpoint xy %f, %f\n", embedstr, gno,
                                                                    dsx, dsy);
            strcpy(tmpstr1, get_format_types(fx));
            strcpy(tmpstr2, get_format_types(fy));
            sprintf(pp+strlen(pp), "%sg%1d fixedpoint format %s %s\n", embedstr, gno,
                                                            tmpstr1, tmpstr2);
            sprintf(pp+strlen(pp), "%sg%1d fixedpoint prec %d, %d\n", embedstr, gno,
                                                                      px, py);

            sprintf(pp+strlen(pp), "%swith g%1d\n", embedstr, gno);

            get_graph_world(gno, &w);
            sprintf(pp+strlen(pp), "%s    world %.16g, %.16g, %.16g, %.16g\n", embedstr,
                w.xg1, w.yg1, w.xg2, w.yg2);

            for (i = 0; i < (unsigned int)graph_world_stack_size(gno); i++) {
                get_world_stack_entry(gno, i, &ws);
                sprintf(pp+strlen(pp), "%s    stack world %.9g, %.9g, %.9g, %.9g\n", embedstr,
                        ws.w.xg1, ws.w.xg2, ws.w.yg1, ws.w.yg2);
            }

            sprintf(pp+strlen(pp), "%s    znorm %g\n", embedstr, get_graph_znorm(gno));

            get_graph_viewport(gno, &v);
            sprintf(pp+strlen(pp), "%s    view %f, %f, %f, %f\n", embedstr,
                v.xv1, v.yv1, v.xv2, v.yv2);

            get_graph_labels(gno, &lab);
            sprintf(pp+strlen(pp), "%s    title \"%s\"\n", embedstr, PSTRING(lab.title.s_plotstring));
            sprintf(pp+strlen(pp), "%s    title font %d\n", embedstr, get_font_mapped_id(lab.title.font));
            sprintf(pp+strlen(pp), "%s    title size %f\n", embedstr, lab.title.charsize);
            sprintf(pp+strlen(pp), "%s    title color %d\n", embedstr, lab.title.color);
            sprintf(pp+strlen(pp), "%s    subtitle \"%s\"\n", embedstr, PSTRING(lab.stitle.s_plotstring));
            sprintf(pp+strlen(pp), "%s    subtitle font %d\n", embedstr, get_font_mapped_id(lab.stitle.font));
            sprintf(pp+strlen(pp), "%s    subtitle size %f\n", embedstr, lab.stitle.charsize);
            sprintf(pp+strlen(pp), "%s    subtitle color %d\n", embedstr, lab.stitle.color);

            sprintf(pp+strlen(pp), "%s    xaxes scale %s\n", embedstr, scale_types(get_graph_xscale(gno)));
            sprintf(pp+strlen(pp), "%s    yaxes scale %s\n", embedstr, scale_types(get_graph_yscale(gno)));
            sprintf(pp+strlen(pp), "%s    xaxes invert %s\n", embedstr, on_or_off(is_graph_xinvert(gno)));
            sprintf(pp+strlen(pp), "%s    yaxes invert %s\n", embedstr, on_or_off(is_graph_yinvert(gno)));

            for (i = 0; i < MAXAXES; i++) {
                t = get_graph_tickmarks(gno, i);
                switch (i) {
                case 0:
                    sprintf(buf, "%s    xaxis ", embedstr);
                    break;
                case 1:
                    sprintf(buf, "%s    yaxis ", embedstr);
                    break;
                case 2:
                    sprintf(buf, "%s    altxaxis ", embedstr);
                    break;
                case 3:
                    sprintf(buf, "%s    altyaxis ", embedstr);
                    break;
                }

                sprintf(pp+strlen(pp), "%s %s\n", buf, on_or_off(t && t->active));
                if (!t || t->active == FALSE) {
                    continue;
                }

                sprintf(pp+strlen(pp), "%s type zero %s\n", buf, true_or_false(t->zero));

                sprintf(pp+strlen(pp), "%s offset %f , %f\n", buf, t->offsx, t->offsy);

                sprintf(pp+strlen(pp), "%s bar %s\n", buf, on_or_off(t->t_drawbar));
                sprintf(pp+strlen(pp), "%s bar color %d\n", buf, t->t_drawbarcolor);
                sprintf(pp+strlen(pp), "%s bar linestyle %d\n", buf, t->t_drawbarlines);
                sprintf(pp+strlen(pp), "%s bar linewidth %.1f\n", buf, t->t_drawbarlinew);


                sprintf(pp+strlen(pp), "%s label \"%s\"\n", buf, PSTRING(t->label.s_plotstring));
                if (t->label_layout == LAYOUT_PERPENDICULAR) {
                    sprintf(pp+strlen(pp), "%s label layout perp\n", buf);
                } else {
                    sprintf(pp+strlen(pp), "%s label layout para\n", buf);
                }
                if (t->label_place == TYPE_AUTO) {
                    sprintf(pp+strlen(pp), "%s label place auto\n", buf);
                } else {
                    sprintf(pp+strlen(pp), "%s label place spec\n", buf);
                    sprintf(pp+strlen(pp), "%s label place %f, %f\n", buf, t->label.x, t->label.y);
                }
                sprintf(pp+strlen(pp), "%s label char size %f\n", buf, t->label.charsize);
                sprintf(pp+strlen(pp), "%s label font %d\n", buf, get_font_mapped_id(t->label.font));
                sprintf(pp+strlen(pp), "%s label color %d\n", buf, t->label.color);
                switch (t->label_op) {
                case PLACEMENT_NORMAL:
                    sprintf(pp+strlen(pp), "%s label place normal\n", buf);
                    break;
                case PLACEMENT_OPPOSITE:
                    sprintf(pp+strlen(pp), "%s label place opposite\n", buf);
                    break;
                case PLACEMENT_BOTH:
                    sprintf(pp+strlen(pp), "%s label place both\n", buf);
                    break;
                }

                sprintf(pp+strlen(pp), "%s tick %s\n", buf, on_or_off(t->t_flag));
                sprintf(pp+strlen(pp), "%s tick major %.16g\n", buf, t->tmajor);
                sprintf(pp+strlen(pp), "%s tick minor ticks %d\n", buf, t->nminor);
                sprintf(pp+strlen(pp), "%s tick default %d\n", buf, t->t_autonum);
                sprintf(pp+strlen(pp), "%s tick place rounded %s\n", buf, true_or_false(t->t_round));
                switch (t->t_inout) {
                case TICKS_IN:
                    sprintf(pp+strlen(pp), "%s tick in\n", buf);
                    break;
                case TICKS_OUT:
                    sprintf(pp+strlen(pp), "%s tick out\n", buf);
                    break;
                case TICKS_BOTH:
                    sprintf(pp+strlen(pp), "%s tick both\n", buf);
                    break;
                }
                sprintf(pp+strlen(pp), "%s tick major size %f\n", buf, t->props.size);
                sprintf(pp+strlen(pp), "%s tick major color %d\n", buf, t->props.color);
                sprintf(pp+strlen(pp), "%s tick major linewidth %.1f\n", buf, t->props.linew);
                sprintf(pp+strlen(pp), "%s tick major linestyle %d\n", buf, t->props.lines);
                sprintf(pp+strlen(pp), "%s tick major grid %s\n", buf, on_or_off(t->props.gridflag));
                sprintf(pp+strlen(pp), "%s tick minor color %d\n", buf, t->mprops.color);
                sprintf(pp+strlen(pp), "%s tick minor linewidth %.1f\n", buf, t->mprops.linew);
                sprintf(pp+strlen(pp), "%s tick minor linestyle %d\n", buf, t->mprops.lines);
                sprintf(pp+strlen(pp), "%s tick minor grid %s\n", buf, on_or_off(t->mprops.gridflag));
                sprintf(pp+strlen(pp), "%s tick minor size %f\n", buf, t->mprops.size);


                sprintf(pp+strlen(pp), "%s ticklabel %s\n", buf, on_or_off(t->tl_flag));
                sprintf(pp+strlen(pp), "%s ticklabel format %s\n", buf, get_format_types(t->tl_format));
                sprintf(pp+strlen(pp), "%s ticklabel prec %d\n", buf, t->tl_prec);

                sprintf(pp+strlen(pp), "%s ticklabel formula \"%s\"\n",
                    buf, PSTRING(t->tl_formula));

                sprintf(pp+strlen(pp), "%s ticklabel append \"%s\"\n", buf, PSTRING(t->tl_appstr));
                sprintf(pp+strlen(pp), "%s ticklabel prepend \"%s\"\n", buf, PSTRING(t->tl_prestr));
                sprintf(pp+strlen(pp), "%s ticklabel angle %d\n", buf, t->tl_angle);
                sprintf(pp+strlen(pp), "%s ticklabel skip %d\n", buf, t->tl_skip);
                sprintf(pp+strlen(pp), "%s ticklabel stagger %d\n", buf, t->tl_staggered);
                switch (t->tl_op) {
                case PLACEMENT_NORMAL:
                    sprintf(pp+strlen(pp), "%s ticklabel place normal\n", buf);
                    break;
                case PLACEMENT_OPPOSITE:
                    sprintf(pp+strlen(pp), "%s ticklabel place opposite\n", buf);
                    break;
                case PLACEMENT_BOTH:
                    sprintf(pp+strlen(pp), "%s ticklabel place both\n", buf);
                    break;
                }

                sprintf(pp+strlen(pp), "%s ticklabel offset %s\n", buf,
                                t->tl_gaptype == TYPE_AUTO ? "auto" : "spec");

                sprintf(pp+strlen(pp), "%s ticklabel offset %f , %f\n", buf, t->tl_gap.x, t->tl_gap.y);
                sprintf(pp+strlen(pp), "%s ticklabel start type %s\n", buf,
                                t->tl_starttype == TYPE_AUTO ? "auto" : "spec");
                sprintf(pp+strlen(pp), "%s ticklabel start %f\n", buf, t->tl_start);
                sprintf(pp+strlen(pp), "%s ticklabel stop type %s\n", buf,
                                t->tl_stoptype == TYPE_AUTO ? "auto" : "spec");
                sprintf(pp+strlen(pp), "%s ticklabel stop %f\n", buf, t->tl_stop);
                sprintf(pp+strlen(pp), "%s ticklabel char size %f\n", buf, t->tl_charsize);
                sprintf(pp+strlen(pp), "%s ticklabel font %d\n", buf, get_font_mapped_id(t->tl_font));
                sprintf(pp+strlen(pp), "%s ticklabel color %d\n", buf, t->tl_color);

                switch (t->t_op) {
                case PLACEMENT_NORMAL:
                    sprintf(pp+strlen(pp), "%s tick place normal\n", buf);
                    break;
                case PLACEMENT_OPPOSITE:
                    sprintf(pp+strlen(pp), "%s tick place opposite\n", buf);
                    break;
                case PLACEMENT_BOTH:
                    sprintf(pp+strlen(pp), "%s tick place both\n", buf);
                    break;
                }
                switch (t->t_spec) {
                case TICKS_SPEC_NONE:
                    sprintf(pp+strlen(pp), "%s tick spec type none\n", buf);
                    break;
                case TICKS_SPEC_MARKS:
                    sprintf(pp+strlen(pp), "%s tick spec type ticks\n", buf);
                    break;
                case TICKS_SPEC_BOTH:
                    sprintf(pp+strlen(pp), "%s tick spec type both\n", buf);
                    break;
                }

                if (t->t_spec != TICKS_SPEC_NONE) {
                    sprintf(pp+strlen(pp), "%s tick spec %d\n", buf, t->nticks);
                    for (j = 0; j < t->nticks; j++) {
                        sprintf(tmpstr1, sformat, t->tloc[j].wtpos);
                        if (t->tloc[j].type == TICK_TYPE_MAJOR) {
                            sprintf(pp+strlen(pp), "%s tick major %d, %s\n",
                                buf, j, tmpstr1);
                            if (t->t_spec == TICKS_SPEC_BOTH) {
                                sprintf(pp+strlen(pp), "%s ticklabel %d, \"%s\"\n",
                                    buf, j, PSTRING(t->tloc[j].label));
                            }
                        } else {
                            sprintf(pp+strlen(pp), "%s tick minor %d, %s\n",
                                buf, j, tmpstr1);
                        }
                    }
                }
            }

            get_graph_legend(gno, &leg);
            sprintf(pp+strlen(pp), "%s    legend %s\n", embedstr, on_or_off(leg.active));
            sprintf(pp+strlen(pp), "%s    legend loctype %s\n", embedstr, w_or_v(leg.loctype));
            sprintf(pp+strlen(pp), "%s    legend %.16g, %.16g\n", embedstr, leg.legx, leg.legy);
            sprintf(pp+strlen(pp), "%s    legend box color %d\n", embedstr, leg.boxpen.color);
            sprintf(pp+strlen(pp), "%s    legend box pattern %d\n", embedstr, leg.boxpen.pattern);
            sprintf(pp+strlen(pp), "%s    legend box linewidth %.1f\n", embedstr, leg.boxlinew);
            sprintf(pp+strlen(pp), "%s    legend box linestyle %d\n", embedstr, leg.boxlines);
            sprintf(pp+strlen(pp), "%s    legend box fill color %d\n", embedstr, leg.boxfillpen.color);
            sprintf(pp+strlen(pp), "%s    legend box fill pattern %d\n", embedstr, leg.boxfillpen.pattern);
            sprintf(pp+strlen(pp), "%s    legend font %d\n", embedstr, get_font_mapped_id(leg.font));
            sprintf(pp+strlen(pp), "%s    legend char size %f\n", embedstr, leg.charsize);
            sprintf(pp+strlen(pp), "%s    legend color %d\n", embedstr, leg.color);
            sprintf(pp+strlen(pp), "%s    legend length %d\n", embedstr, leg.len);
            sprintf(pp+strlen(pp), "%s    legend vgap %d\n", embedstr, leg.vgap);
            sprintf(pp+strlen(pp), "%s    legend hgap %d\n", embedstr, leg.hgap);
            sprintf(pp+strlen(pp), "%s    legend invert %s\n", embedstr, true_or_false(leg.invert));


            get_graph_framep(gno, &f);
            sprintf(pp+strlen(pp), "%s    frame type %d\n", embedstr, f.type);
            sprintf(pp+strlen(pp), "%s    frame linestyle %d\n", embedstr, f.lines);
            sprintf(pp+strlen(pp), "%s    frame linewidth %.1f\n", embedstr, f.linew);
            sprintf(pp+strlen(pp), "%s    frame color %d\n", embedstr, f.pen.color);
            sprintf(pp+strlen(pp), "%s    frame pattern %d\n", embedstr, f.pen.pattern);
            sprintf(pp+strlen(pp), "%s    frame background color %d\n", embedstr, f.fillpen.color);
            sprintf(pp+strlen(pp), "%s    frame background pattern %d\n", embedstr, f.fillpen.pattern);

            for (i = 0; i < (unsigned int)number_of_sets(gno); i++) {
                get_graph_plotarr(gno, i, &p);
                if (is_set_active(gno, i) == TRUE) {
                    sprintf(pp+strlen(pp), "%s    s%1d hidden %s\n", embedstr, i,
                                            true_or_false(p.hidden));
                    sprintf(pp+strlen(pp), "%s    s%1d type %s\n", embedstr, i, set_types(p.type));
                    sprintf(pp+strlen(pp), "%s    s%1d symbol %d\n", embedstr, i, p.sym);
                    sprintf(pp+strlen(pp), "%s    s%1d symbol size %f\n", embedstr, i, p.symsize);
                    sprintf(pp+strlen(pp), "%s    s%1d symbol color %d\n", embedstr, i, p.sympen.color);
                    sprintf(pp+strlen(pp), "%s    s%1d symbol pattern %d\n", embedstr, i, p.sympen.pattern);
                    sprintf(pp+strlen(pp), "%s    s%1d symbol fill color %d\n", embedstr, i, p.symfillpen.color);
                    sprintf(pp+strlen(pp), "%s    s%1d symbol fill pattern %d\n", embedstr, i, p.symfillpen.pattern);
                    sprintf(pp+strlen(pp), "%s    s%1d symbol linewidth %.1f\n", embedstr, i, p.symlinew);
                    sprintf(pp+strlen(pp), "%s    s%1d symbol linestyle %d\n", embedstr, i, p.symlines);
                    sprintf(pp+strlen(pp), "%s    s%1d symbol char %d\n", embedstr, i, p.symchar);
                    sprintf(pp+strlen(pp), "%s    s%1d symbol char font %d\n", embedstr, i, get_font_mapped_id(p.charfont));
                    sprintf(pp+strlen(pp), "%s    s%1d symbol skip %d\n", embedstr, i, p.symskip);

                    sprintf(pp+strlen(pp), "%s    s%1d line type %d\n", embedstr, i, p.linet);
                    sprintf(pp+strlen(pp), "%s    s%1d line linestyle %d\n", embedstr, i, p.lines);
                    sprintf(pp+strlen(pp), "%s    s%1d line linewidth %.1f\n", embedstr, i, p.linew);
                    sprintf(pp+strlen(pp), "%s    s%1d line color %d\n", embedstr, i, p.linepen.color);
                    sprintf(pp+strlen(pp), "%s    s%1d line pattern %d\n", embedstr, i, p.linepen.pattern);

                    sprintf(pp+strlen(pp), "%s    s%1d baseline type %d\n", embedstr, i, p.baseline_type);
                    sprintf(pp+strlen(pp), "%s    s%1d baseline %s\n", embedstr, i, on_or_off(p.baseline));

                    sprintf(pp+strlen(pp), "%s    s%1d dropline %s\n", embedstr, i, on_or_off(p.dropline));

                    sprintf(pp+strlen(pp), "%s    s%1d fill type %d\n", embedstr, i, p.filltype);
                    sprintf(pp+strlen(pp), "%s    s%1d fill rule %d\n", embedstr, i, p.fillrule);
                    sprintf(pp+strlen(pp), "%s    s%1d fill color %d\n", embedstr, i, p.setfillpen.color);
                    sprintf(pp+strlen(pp), "%s    s%1d fill pattern %d\n", embedstr, i, p.setfillpen.pattern);

                    sprintf(pp+strlen(pp), "%s    s%1d avalue %s\n", embedstr, i, on_or_off(p.avalue.active));
                    sprintf(pp+strlen(pp), "%s    s%1d avalue type %d\n", embedstr, i, p.avalue.type);
                    sprintf(pp+strlen(pp), "%s    s%1d avalue char size %f\n", embedstr, i, p.avalue.size);
                    sprintf(pp+strlen(pp), "%s    s%1d avalue font %d\n", embedstr, i, get_font_mapped_id(p.avalue.font));
                    sprintf(pp+strlen(pp), "%s    s%1d avalue color %d\n", embedstr, i, p.avalue.color);
                    sprintf(pp+strlen(pp), "%s    s%1d avalue rot %d\n", embedstr, i, p.avalue.angle);
                    sprintf(pp+strlen(pp), "%s    s%1d avalue format %s\n", embedstr, i, get_format_types(p.avalue.format));
                    sprintf(pp+strlen(pp), "%s    s%1d avalue prec %d\n", embedstr, i, p.avalue.prec);
                    sprintf(pp+strlen(pp), "%s    s%1d avalue prepend \"%s\"\n", embedstr, i, PSTRING(p.avalue.prestr));
                    sprintf(pp+strlen(pp), "%s    s%1d avalue append \"%s\"\n", embedstr, i, PSTRING(p.avalue.appstr));
                    sprintf(pp+strlen(pp), "%s    s%1d avalue offset %f , %f\n", embedstr, i, p.avalue.offset.x, p.avalue.offset.y);

                    sprintf(pp+strlen(pp), "%s    s%1d errorbar %s\n", embedstr, i, on_or_off(p.errbar.active));
                    switch (p.errbar.ptype) {
                    case PLACEMENT_NORMAL:
                        sprintf(pp+strlen(pp), "%s    s%1d errorbar place normal\n", embedstr, i);
                        break;
                    case PLACEMENT_OPPOSITE:
                        sprintf(pp+strlen(pp), "%s    s%1d errorbar place opposite\n", embedstr, i);
                        break;
                    case PLACEMENT_BOTH:
                        sprintf(pp+strlen(pp), "%s    s%1d errorbar place both\n", embedstr, i);
                        break;
                    }
                    sprintf(pp+strlen(pp), "%s    s%1d errorbar color %d\n", embedstr, i, p.errbar.pen.color);
                    sprintf(pp+strlen(pp), "%s    s%1d errorbar pattern %d\n", embedstr, i, p.errbar.pen.pattern);
                    sprintf(pp+strlen(pp), "%s    s%1d errorbar size %f\n", embedstr, i, p.errbar.barsize);
                    sprintf(pp+strlen(pp), "%s    s%1d errorbar linewidth %.1f\n", embedstr, i, p.errbar.linew);
                    sprintf(pp+strlen(pp), "%s    s%1d errorbar linestyle %d\n", embedstr, i, p.errbar.lines);
                    sprintf(pp+strlen(pp), "%s    s%1d errorbar riser linewidth %.1f\n", embedstr, i, p.errbar.riser_linew);
                    sprintf(pp+strlen(pp), "%s    s%1d errorbar riser linestyle %d\n", embedstr, i, p.errbar.riser_lines);
                    sprintf(pp+strlen(pp), "%s    s%1d errorbar riser clip %s\n", embedstr, i, on_or_off(p.errbar.arrow_clip));
                    sprintf(pp+strlen(pp), "%s    s%1d errorbar riser clip length %f\n", embedstr, i, p.errbar.cliplen);

                    if (is_hotlinked(gno, i)) {
                        sprintf(pp+strlen(pp), "%s    s%1d link %s \"%s\"\n", embedstr, i,
                                p.hotsrc == SOURCE_DISK ? "disk" : "pipe", p.hotfile);
                    }

                    sprintf(pp+strlen(pp), "%s    s%1d comment \"%s\"\n", embedstr, i, PSTRING(p.comments));

                    sprintf(pp+strlen(pp), "%s    s%1d legend  \"%s\"\n", embedstr, i, PSTRING(p.lstr));
                }
            }
        }
    }
}

static void put_beast_objects(int gno, char * pp, int embed)
{
    int i;
    boxtype b;
    linetype l;
    ellipsetype e;
    plotstr s;
    char embedstr[2];
    (void)gno;

    if (embed) {
        strcpy(embedstr, "@");
    } else {
        embedstr[0] = 0;
    }
    for (i = 0; i < number_of_boxes(); i++) {
        get_graph_box(i, &b);
        if (b.active == TRUE) {
            sprintf(pp+strlen(pp), "%swith box\n", embedstr);
            sprintf(pp+strlen(pp), "%s    box on\n", embedstr);
            sprintf(pp+strlen(pp), "%s    box loctype %s\n", embedstr, w_or_v(b.loctype));
            if (b.loctype == COORD_WORLD) {
                sprintf(pp+strlen(pp), "%s    box g%1d\n", embedstr, b.gno);
            }
            sprintf(pp+strlen(pp), "%s    box %.16g, %.16g, %.16g, %.16g\n", embedstr, b.x1, b.y1, b.x2, b.y2);
            sprintf(pp+strlen(pp), "%s    box linestyle %d\n", embedstr, b.lines);
            sprintf(pp+strlen(pp), "%s    box linewidth %.1f\n", embedstr, b.linew);
            sprintf(pp+strlen(pp), "%s    box color %d\n", embedstr, b.color);
            sprintf(pp+strlen(pp), "%s    box fill color %d\n", embedstr, b.fillcolor);
            sprintf(pp+strlen(pp), "%s    box fill pattern %d\n", embedstr, b.fillpattern);
            sprintf(pp+strlen(pp), "%sbox def\n", embedstr);
        }
    }

    for (i = 0; i < number_of_ellipses(); i++) {
        get_graph_ellipse(i, &e);
        if (e.active == TRUE) {
            sprintf(pp+strlen(pp), "%swith ellipse\n", embedstr);
            sprintf(pp+strlen(pp), "%s    ellipse on\n", embedstr);
            sprintf(pp+strlen(pp), "%s    ellipse loctype %s\n", embedstr, w_or_v(e.loctype));
            if (e.loctype == COORD_WORLD) {
                sprintf(pp+strlen(pp), "%s    ellipse g%1d\n", embedstr, e.gno);
            }
            sprintf(pp+strlen(pp), "%s    ellipse %.16g, %.16g, %.16g, %.16g\n", embedstr, e.x1, e.y1, e.x2, e.y2);
            sprintf(pp+strlen(pp), "%s    ellipse linestyle %d\n", embedstr, e.lines);
            sprintf(pp+strlen(pp), "%s    ellipse linewidth %.1f\n", embedstr, e.linew);
            sprintf(pp+strlen(pp), "%s    ellipse color %d\n", embedstr, e.color);
            sprintf(pp+strlen(pp), "%s    ellipse fill color %d\n", embedstr, e.fillcolor);
            sprintf(pp+strlen(pp), "%s    ellipse fill pattern %d\n", embedstr, e.fillpattern);
            sprintf(pp+strlen(pp), "%sellipse def\n", embedstr);
        }
    }
   for (i = 0; i < number_of_lines(); i++) {
        get_graph_line(i, &l);
        if (l.active == TRUE) {
            sprintf(pp+strlen(pp), "%swith line\n", embedstr);
            sprintf(pp+strlen(pp), "%s    line on\n", embedstr);
            sprintf(pp+strlen(pp), "%s    line loctype %s\n", embedstr, w_or_v(l.loctype));
            if (l.loctype == COORD_WORLD) {
                sprintf(pp+strlen(pp), "%s    line g%1d\n", embedstr, l.gno);
            }
            sprintf(pp+strlen(pp), "%s    line %.16g, %.16g, %.16g, %.16g\n", embedstr, l.x1, l.y1, l.x2, l.y2);
            sprintf(pp+strlen(pp), "%s    line linewidth %.1f\n", embedstr, l.linew);
            sprintf(pp+strlen(pp), "%s    line linestyle %d\n", embedstr, l.lines);
            sprintf(pp+strlen(pp), "%s    line color %d\n", embedstr, l.color);
            sprintf(pp+strlen(pp), "%s    line arrow %d\n", embedstr, l.arrow_end);
            sprintf(pp+strlen(pp), "%s    line arrow type %d\n", embedstr, l.arrow.type);
            sprintf(pp+strlen(pp), "%s    line arrow length %f\n", embedstr, l.arrow.length);
            sprintf(pp+strlen(pp), "%s    line arrow layout %f, %f\n", embedstr, l.arrow.dL_ff, l.arrow.lL_ff);
            sprintf(pp+strlen(pp), "%sline def\n", embedstr);
        }
    }

    for (i = 0; i < number_of_strings(); i++) {
        get_graph_string(i, &s);
        if (s.active == TRUE && s.s_plotstring[0]) {
            sprintf(pp+strlen(pp), "%swith string\n", embedstr);
            sprintf(pp+strlen(pp), "%s    string on\n", embedstr);
            sprintf(pp+strlen(pp), "%s    string loctype %s\n", embedstr, w_or_v(s.loctype));
            if (s.loctype == COORD_WORLD) {
                sprintf(pp+strlen(pp), "%s    string g%1d\n", embedstr, s.gno);
            }
            sprintf(pp+strlen(pp), "%s    string %.16g, %.16g\n", embedstr, s.x, s.y);
            sprintf(pp+strlen(pp), "%s    string color %d\n", embedstr, s.color);
            sprintf(pp+strlen(pp), "%s    string rot %d\n", embedstr, s.rot);
            sprintf(pp+strlen(pp), "%s    string font %d\n", embedstr, get_font_mapped_id(s.font));
            sprintf(pp+strlen(pp), "%s    string just %d\n", embedstr, s.just);
            sprintf(pp+strlen(pp), "%s    string char size %f\n", embedstr, s.charsize);
            sprintf(pp+strlen(pp), "%s    string def \"%s\"\n", embedstr, PSTRING(s.s_plotstring));
        }
    }
}

static void put_beast_regions(char * pp, int embed)
{
    int i, j;
    char embedstr[2];

    if (embed) {
        strcpy(embedstr, "@");
    } else {
        embedstr[0] = 0;
    }
    for (i = 0; i < MAXREGION; i++) {
      sprintf(pp+strlen(pp), "%sr%1d %s\n", embedstr, i, on_or_off(rg[i].active));

      sprintf(pp+strlen(pp), "%slink r%1d to g%1d\n", embedstr, i, rg[i].linkto);

      switch (rg[i].type) {
      case REGION_ABOVE:
    sprintf(pp+strlen(pp), "%sr%1d type above\n", embedstr, i);
    break;
      case REGION_BELOW:
    sprintf(pp+strlen(pp), "%sr%1d type below\n", embedstr, i);
    break;
      case REGION_TOLEFT:
    sprintf(pp+strlen(pp), "%sr%1d type left\n", embedstr, i);
    break;
      case REGION_TORIGHT:
    sprintf(pp+strlen(pp), "%sr%1d type right\n", embedstr, i);
    break;
      case REGION_POLYI:
    sprintf(pp+strlen(pp), "%sr%1d type polyi\n", embedstr, i);
    break;
      case REGION_POLYO:
    sprintf(pp+strlen(pp), "%sr%1d type polyo\n", embedstr, i);
    break;
      case REGION_HORIZI:
    sprintf(pp+strlen(pp), "%sr%1d type horizi\n", embedstr, i);
    break;
      case REGION_VERTI:
    sprintf(pp+strlen(pp),"%sr%1d type verti\n", embedstr, i);
    break;
      case REGION_HORIZO:
    sprintf(pp+strlen(pp), "%sr%1d type horizo\n", embedstr, i);
    break;
      case REGION_VERTO:
    sprintf(pp+strlen(pp),"%sr%1d type verto\n", embedstr, i);
    break;
      }
      sprintf(pp+strlen(pp), "%sr%1d linestyle %d\n", embedstr, i, rg[i].lines);
      sprintf(pp+strlen(pp), "%sr%1d linewidth %.1f\n", embedstr, i, rg[i].linew);
      sprintf(pp+strlen(pp), "%sr%1d color %d\n", embedstr, i, rg[i].color);
      if (rg[i].type != REGION_POLYI && rg[i].type != REGION_POLYO) {
    sprintf(pp+strlen(pp), "%sr%1d line %.16g, %.16g, %.16g, %.16g\n", embedstr, i, rg[i].x1, rg[i].y1, rg[i].x2, rg[i].y2);
      } else {
    if (rg[i].x != NULL) {
      for (j = 0; j < rg[i].n; j++) {
        sprintf(pp+strlen(pp), "%sr%1d xy %.16g, %.16g\n", embedstr, i, rg[i].x[j], rg[i].y[j]);
      }
    }
      }
    }
}

void put_fitparmsbeast(char * pp, int embed)
{
    int i;
    char embedstr[2];

    if (embed) {
        strcpy(embedstr, "@");
    } else {
        embedstr[0] = 0;
    }

    sprintf(pp+strlen(pp), "# Grace fit description file\n");
    sprintf(pp+strlen(pp), "#\n");

    sprintf(pp+strlen(pp), "%sfit title \"%s\"\n", embedstr, PSTRING(nonl_opts.title));
    sprintf(pp+strlen(pp), "%sfit formula \"%s\"\n", embedstr, PSTRING(nonl_opts.formula));
    sprintf(pp+strlen(pp), "%sfit with %1d parameters\n", embedstr, nonl_opts.parnum);
    sprintf(pp+strlen(pp), "%sfit prec %g\n", embedstr, nonl_opts.tolerance);

    for (i = 0; i < nonl_opts.parnum; i++) {
        sprintf(pp+strlen(pp), "%sa%1d = %g\n", embedstr, i, nonl_parms[i].value);
        if (nonl_parms[i].constr) {
            sprintf(pp+strlen(pp), "%sa%1d constraints on\n", embedstr, i);
        } else {
            sprintf(pp+strlen(pp), "%sa%1d constraints off\n", embedstr, i);
        }
        sprintf(pp+strlen(pp), "%sa%1dmin = %g\n", embedstr, i, nonl_parms[i].min);
        sprintf(pp+strlen(pp), "%sa%1dmax = %g\n", embedstr, i, nonl_parms[i].max);
    }
}

void putparms(int gno, FILE *pp, int embed)
{
    unsigned int i;
    int j, k, ming, maxg;
    int ps, pt, gh, gt, fx, fy, px, py;
    double dsx, dsy;
    char embedstr[2], tmpstr1[64], tmpstr2[64];
    framep f;
    legend leg;
    labels lab;
    plotarr p;
    tickmarks *t;
    world_stack ws;
    world w;
    view v;
    CMap_entry *cmap;
    GLocator locator;
    char *p1, *p2, *tmpbuf;
    int wpp, hpp;

    if (embed) {
        strcpy(embedstr, "@");
    } else {
        embedstr[0] = 0;
    }
    fprintf(pp, "# Grace project file\n");
    fprintf(pp, "#\n");

    /* Print some global variables */
    fprintf(pp, "%sversion %ld\n", embedstr, bi_version_id());

    get_device_page_dimensions(tdevice, &wpp, &hpp);
    fprintf(pp, "%spage size %d, %d\n", embedstr, wpp, hpp);

    tmpbuf = copy_string(NULL, get_project_description());
    if (tmpbuf != NULL) {
        p1 = tmpbuf;
        while ((p2 = strchr (p1, '\n')) != NULL) {
            *p2 = 0;
            fprintf (pp, "%sdescription \"%s\"\n", embedstr, PSTRING(p1));
            *p2 = '\n';
            p1 = p2;
            p1++;
        }
        if (*p1) {
            fprintf (pp, "%sdescription \"%s\"\n", embedstr, PSTRING(p1));
        }
        xfree(tmpbuf);
    }

    fprintf(pp, "%spage scroll %d%%\n", embedstr, (int) rint(scrollper * 100));
    fprintf(pp, "%spage inout %d%%\n", embedstr, (int) rint(shexper * 100));
    fprintf(pp, "%slink page %s\n", embedstr, scrolling_islinked ? "on" : "off");

    for (i = 0; i < number_of_fonts(); i++) {
        if (get_font_mapped_id(i) != BAD_FONT_ID) {
            fprintf(pp, "%smap font %d to \"%s\", \"%s\"\n", embedstr,
                                                    get_font_mapped_id(i),
                                                    get_fontalias(i),
                                                    get_fontfallback(i));
        }
    }

    for (i = 0; i < number_of_colors(); i++) {
        cmap = get_cmap_entry(i);
        if (cmap != NULL && cmap->ctype == COLOR_MAIN) {
            fprintf(pp, "%smap color %d to (%d, %d, %d), \"%s\"\n", embedstr,
                i, cmap->rgb.red, cmap->rgb.green, cmap->rgb.blue,
                PSTRING(cmap->cname));
        }
    }
    
    fprintf(pp, "%sreference date %.16g\n", embedstr, get_ref_date());
    fprintf(pp, "%sdate wrap %s\n", embedstr,
        on_or_off(two_digits_years_allowed()));
    fprintf(pp, "%sdate wrap year %d\n", embedstr, get_wrap_year());
    
    fprintf(pp, "%sdefault linewidth %.1f\n", embedstr, grdefaults.linew);
    fprintf(pp, "%sdefault linestyle %d\n", embedstr, grdefaults.lines);
    fprintf(pp, "%sdefault color %d\n", embedstr, grdefaults.color);
    fprintf(pp, "%sdefault pattern %d\n", embedstr, grdefaults.pattern);
    fprintf(pp, "%sdefault font %d\n", embedstr, get_font_mapped_id(grdefaults.font));
    fprintf(pp, "%sdefault char size %f\n", embedstr, grdefaults.charsize);
    fprintf(pp, "%sdefault symbol size %f\n", embedstr, grdefaults.symsize);
    fprintf(pp, "%sdefault sformat \"%s\"\n", embedstr, PSTRING(sformat));
    
    fprintf(pp, "%sbackground color %d\n", embedstr, getbgcolor());
    fprintf(pp, "%spage background fill %s\n", embedstr, on_or_off(getbgfill()));
            
    fprintf(pp, "%stimestamp %s\n", embedstr, on_or_off(timestamp.active));
    fprintf(pp, "%stimestamp %.16g, %.16g\n", embedstr, timestamp.x, timestamp.y);
    fprintf(pp, "%stimestamp color %d\n", embedstr, timestamp.color);
    fprintf(pp, "%stimestamp rot %d\n", embedstr, timestamp.rot);
    fprintf(pp, "%stimestamp font %d\n", embedstr, get_font_mapped_id(timestamp.font));
    fprintf(pp, "%stimestamp char size %f\n", embedstr, timestamp.charsize);
    fprintf(pp, "%stimestamp def \"%s\"\n", embedstr, PSTRING(timestamp.s_plotstring));


    put_objects(gno, pp, embed);
    
    put_regions(pp, embed);
    
    if (gno == ALL_GRAPHS) {
        maxg = number_of_graphs() - 1;
        ming = 0;
    } else {
        maxg = gno;
        ming = gno;
    }
    for (k = ming; k <= maxg; k++) {
        if (is_graph_active(k)) {
            gno = k;
            gh = is_graph_hidden(gno);
            gt = get_graph_type(gno);
            get_graph_locator(gno, &locator);
            ps = locator.pointset;
            pt = locator.pt_type;
            dsx = locator.dsx;
            dsy = locator.dsy;
            fx = locator.fx;
            fy = locator.fy;
            px = locator.px;
            py = locator.py;

            fprintf(pp, "%sg%1d %s\n", embedstr, gno, 
                                            on_or_off(is_graph_active(gno)));
            fprintf(pp, "%sg%1d hidden %s\n", embedstr, gno,
                                            true_or_false(gh));
            fprintf(pp, "%sg%1d type %s\n", embedstr, gno, graph_types(gt));
            fprintf(pp, "%sg%1d stacked %s\n", embedstr, gno,
                                        true_or_false(is_graph_stacked(gno)));

            fprintf(pp, "%sg%1d bar hgap %f\n", embedstr, gno,
                                                        get_graph_bargap(gno));

            fprintf(pp, "%sg%1d fixedpoint %s\n", embedstr, gno, on_or_off(ps));
            fprintf(pp, "%sg%1d fixedpoint type %d\n", embedstr, gno, pt);
            fprintf(pp, "%sg%1d fixedpoint xy %f, %f\n", embedstr, gno,
                                                                    dsx, dsy);
            strcpy(tmpstr1, get_format_types(fx));
            strcpy(tmpstr2, get_format_types(fy));
            fprintf(pp, "%sg%1d fixedpoint format %s %s\n", embedstr, gno,
                                                            tmpstr1, tmpstr2);
            fprintf(pp, "%sg%1d fixedpoint prec %d, %d\n", embedstr, gno,
                                                                      px, py);

            fprintf(pp, "%swith g%1d\n", embedstr, gno);

            get_graph_world(gno, &w);
            fprintf(pp, "%s    world %.16g, %.16g, %.16g, %.16g\n", embedstr,
                w.xg1, w.yg1, w.xg2, w.yg2);

            for (i = 0; i < (unsigned int)graph_world_stack_size(gno); i++) {
                get_world_stack_entry(gno, i, &ws);
                fprintf(pp, "%s    stack world %.9g, %.9g, %.9g, %.9g\n", embedstr,
                        ws.w.xg1, ws.w.xg2, ws.w.yg1, ws.w.yg2);
            }

            fprintf(pp, "%s    znorm %g\n", embedstr, get_graph_znorm(gno));

            get_graph_viewport(gno, &v);
            fprintf(pp, "%s    view %f, %f, %f, %f\n", embedstr,
                v.xv1, v.yv1, v.xv2, v.yv2);

            get_graph_labels(gno, &lab);
            fprintf(pp, "%s    title \"%s\"\n", embedstr, PSTRING(lab.title.s_plotstring));
            fprintf(pp, "%s    title font %d\n", embedstr, get_font_mapped_id(lab.title.font));
            fprintf(pp, "%s    title size %f\n", embedstr, lab.title.charsize);
            fprintf(pp, "%s    title color %d\n", embedstr, lab.title.color);
            fprintf(pp, "%s    subtitle \"%s\"\n", embedstr, PSTRING(lab.stitle.s_plotstring));
            fprintf(pp, "%s    subtitle font %d\n", embedstr, get_font_mapped_id(lab.stitle.font));
            fprintf(pp, "%s    subtitle size %f\n", embedstr, lab.stitle.charsize);
            fprintf(pp, "%s    subtitle color %d\n", embedstr, lab.stitle.color);

            fprintf(pp, "%s    xaxes scale %s\n", embedstr, scale_types(get_graph_xscale(gno)));
            fprintf(pp, "%s    yaxes scale %s\n", embedstr, scale_types(get_graph_yscale(gno)));
            fprintf(pp, "%s    xaxes invert %s\n", embedstr, on_or_off(is_graph_xinvert(gno)));
            fprintf(pp, "%s    yaxes invert %s\n", embedstr, on_or_off(is_graph_yinvert(gno)));

            for (i = 0; i < MAXAXES; i++) {
                t = get_graph_tickmarks(gno, i);
                switch (i) {
                case 0:
                    sprintf(buf, "%s    xaxis ", embedstr);
                    break;
                case 1:
                    sprintf(buf, "%s    yaxis ", embedstr);
                    break;
                case 2:
                    sprintf(buf, "%s    altxaxis ", embedstr);
                    break;
                case 3:
                    sprintf(buf, "%s    altyaxis ", embedstr);
                    break;
                }

                fprintf(pp, "%s %s\n", buf, on_or_off(t && t->active));
                if (!t || t->active == FALSE) {
                    continue;
                }
                
                fprintf(pp, "%s type zero %s\n", buf, true_or_false(t->zero));

                fprintf(pp, "%s offset %f , %f\n", buf, t->offsx, t->offsy);

                fprintf(pp, "%s bar %s\n", buf, on_or_off(t->t_drawbar));
                fprintf(pp, "%s bar color %d\n", buf, t->t_drawbarcolor);
                fprintf(pp, "%s bar linestyle %d\n", buf, t->t_drawbarlines);
                fprintf(pp, "%s bar linewidth %.1f\n", buf, t->t_drawbarlinew);


                fprintf(pp, "%s label \"%s\"\n", buf, PSTRING(t->label.s_plotstring));
                if (t->label_layout == LAYOUT_PERPENDICULAR) {
                    fprintf(pp, "%s label layout perp\n", buf);
                } else {
                    fprintf(pp, "%s label layout para\n", buf);
                }
                if (t->label_place == TYPE_AUTO) {
                    fprintf(pp, "%s label place auto\n", buf);
                } else {
                    fprintf(pp, "%s label place spec\n", buf);
                    fprintf(pp, "%s label place %f, %f\n", buf, t->label.x, t->label.y);
                }
                fprintf(pp, "%s label char size %f\n", buf, t->label.charsize);
                fprintf(pp, "%s label font %d\n", buf, get_font_mapped_id(t->label.font));
                fprintf(pp, "%s label color %d\n", buf, t->label.color);
                switch (t->label_op) {
                case PLACEMENT_NORMAL:
                    fprintf(pp, "%s label place normal\n", buf);
                    break;
                case PLACEMENT_OPPOSITE:
                    fprintf(pp, "%s label place opposite\n", buf);
                    break;
                case PLACEMENT_BOTH:
                    fprintf(pp, "%s label place both\n", buf);
                    break;
                }

                fprintf(pp, "%s tick %s\n", buf, on_or_off(t->t_flag));
                fprintf(pp, "%s tick major %.16g\n", buf, t->tmajor);
                fprintf(pp, "%s tick minor ticks %d\n", buf, t->nminor);
                fprintf(pp, "%s tick default %d\n", buf, t->t_autonum);
                fprintf(pp, "%s tick place rounded %s\n", buf, true_or_false(t->t_round));
                switch (t->t_inout) {
                case TICKS_IN:
                    fprintf(pp, "%s tick in\n", buf);
                    break;
                case TICKS_OUT:
                    fprintf(pp, "%s tick out\n", buf);
                    break;
                case TICKS_BOTH:
                    fprintf(pp, "%s tick both\n", buf);
                    break;
                }
                fprintf(pp, "%s tick major size %f\n", buf, t->props.size);
                fprintf(pp, "%s tick major color %d\n", buf, t->props.color);
                fprintf(pp, "%s tick major linewidth %.1f\n", buf, t->props.linew);
                fprintf(pp, "%s tick major linestyle %d\n", buf, t->props.lines);
                fprintf(pp, "%s tick major grid %s\n", buf, on_or_off(t->props.gridflag));
                fprintf(pp, "%s tick minor color %d\n", buf, t->mprops.color);
                fprintf(pp, "%s tick minor linewidth %.1f\n", buf, t->mprops.linew);
                fprintf(pp, "%s tick minor linestyle %d\n", buf, t->mprops.lines);
                fprintf(pp, "%s tick minor grid %s\n", buf, on_or_off(t->mprops.gridflag));
                fprintf(pp, "%s tick minor size %f\n", buf, t->mprops.size);


                fprintf(pp, "%s ticklabel %s\n", buf, on_or_off(t->tl_flag));
                fprintf(pp, "%s ticklabel format %s\n", buf, get_format_types(t->tl_format));
                fprintf(pp, "%s ticklabel prec %d\n", buf, t->tl_prec);

                fprintf(pp, "%s ticklabel formula \"%s\"\n",
                    buf, PSTRING(t->tl_formula));

                fprintf(pp, "%s ticklabel append \"%s\"\n", buf, PSTRING(t->tl_appstr));
                fprintf(pp, "%s ticklabel prepend \"%s\"\n", buf, PSTRING(t->tl_prestr));
                fprintf(pp, "%s ticklabel angle %d\n", buf, t->tl_angle);
                fprintf(pp, "%s ticklabel skip %d\n", buf, t->tl_skip);
                fprintf(pp, "%s ticklabel stagger %d\n", buf, t->tl_staggered);
                switch (t->tl_op) {
                case PLACEMENT_NORMAL:
                    fprintf(pp, "%s ticklabel place normal\n", buf);
                    break;
                case PLACEMENT_OPPOSITE:
                    fprintf(pp, "%s ticklabel place opposite\n", buf);
                    break;
                case PLACEMENT_BOTH:
                    fprintf(pp, "%s ticklabel place both\n", buf);
                    break;
                }

                fprintf(pp, "%s ticklabel offset %s\n", buf,
                                t->tl_gaptype == TYPE_AUTO ? "auto" : "spec");

                fprintf(pp, "%s ticklabel offset %f , %f\n", buf, t->tl_gap.x, t->tl_gap.y);
                fprintf(pp, "%s ticklabel start type %s\n", buf,
                                t->tl_starttype == TYPE_AUTO ? "auto" : "spec");
                fprintf(pp, "%s ticklabel start %f\n", buf, t->tl_start);
                fprintf(pp, "%s ticklabel stop type %s\n", buf,
                                t->tl_stoptype == TYPE_AUTO ? "auto" : "spec");
                fprintf(pp, "%s ticklabel stop %f\n", buf, t->tl_stop);
                fprintf(pp, "%s ticklabel char size %f\n", buf, t->tl_charsize);
                fprintf(pp, "%s ticklabel font %d\n", buf, get_font_mapped_id(t->tl_font));
                fprintf(pp, "%s ticklabel color %d\n", buf, t->tl_color);

                switch (t->t_op) {
                case PLACEMENT_NORMAL:
                    fprintf(pp, "%s tick place normal\n", buf);
                    break;
                case PLACEMENT_OPPOSITE:
                    fprintf(pp, "%s tick place opposite\n", buf);
                    break;
                case PLACEMENT_BOTH:
                    fprintf(pp, "%s tick place both\n", buf);
                    break;
                }
                switch (t->t_spec) {
                case TICKS_SPEC_NONE:
                    fprintf(pp, "%s tick spec type none\n", buf);
                    break;
                case TICKS_SPEC_MARKS:
                    fprintf(pp, "%s tick spec type ticks\n", buf);
                    break;
                case TICKS_SPEC_BOTH:
                    fprintf(pp, "%s tick spec type both\n", buf);
                    break;
                }
                
                if (t->t_spec != TICKS_SPEC_NONE) {
                    fprintf(pp, "%s tick spec %d\n", buf, t->nticks);
                    for (j = 0; j < t->nticks; j++) {
                        sprintf(tmpstr1, sformat, t->tloc[j].wtpos);
                        if (t->tloc[j].type == TICK_TYPE_MAJOR) {
                            fprintf(pp, "%s tick major %d, %s\n",
                                buf, j, tmpstr1);
                            if (t->t_spec == TICKS_SPEC_BOTH) {
                                fprintf(pp, "%s ticklabel %d, \"%s\"\n",
                                    buf, j, PSTRING(t->tloc[j].label));
                            }
                        } else {
                            fprintf(pp, "%s tick minor %d, %s\n",
                                buf, j, tmpstr1);
                        }
                    }
                }
            }

            get_graph_legend(gno, &leg);
            fprintf(pp, "%s    legend %s\n", embedstr, on_or_off(leg.active));
            fprintf(pp, "%s    legend loctype %s\n", embedstr, w_or_v(leg.loctype));
            fprintf(pp, "%s    legend %.16g, %.16g\n", embedstr, leg.legx, leg.legy);
            fprintf(pp, "%s    legend box color %d\n", embedstr, leg.boxpen.color);
            fprintf(pp, "%s    legend box pattern %d\n", embedstr, leg.boxpen.pattern);
            fprintf(pp, "%s    legend box linewidth %.1f\n", embedstr, leg.boxlinew);
            fprintf(pp, "%s    legend box linestyle %d\n", embedstr, leg.boxlines);
            fprintf(pp, "%s    legend box fill color %d\n", embedstr, leg.boxfillpen.color);
            fprintf(pp, "%s    legend box fill pattern %d\n", embedstr, leg.boxfillpen.pattern);
            fprintf(pp, "%s    legend font %d\n", embedstr, get_font_mapped_id(leg.font));
            fprintf(pp, "%s    legend char size %f\n", embedstr, leg.charsize);
            fprintf(pp, "%s    legend color %d\n", embedstr, leg.color);
            fprintf(pp, "%s    legend length %d\n", embedstr, leg.len);
            fprintf(pp, "%s    legend vgap %d\n", embedstr, leg.vgap);
            fprintf(pp, "%s    legend hgap %d\n", embedstr, leg.hgap);
            fprintf(pp, "%s    legend invert %s\n", embedstr, true_or_false(leg.invert));


            get_graph_framep(gno, &f);
            fprintf(pp, "%s    frame type %d\n", embedstr, f.type);
            fprintf(pp, "%s    frame linestyle %d\n", embedstr, f.lines);
            fprintf(pp, "%s    frame linewidth %.1f\n", embedstr, f.linew);
            fprintf(pp, "%s    frame color %d\n", embedstr, f.pen.color);
            fprintf(pp, "%s    frame pattern %d\n", embedstr, f.pen.pattern);
            fprintf(pp, "%s    frame background color %d\n", embedstr, f.fillpen.color);
            fprintf(pp, "%s    frame background pattern %d\n", embedstr, f.fillpen.pattern);

            for (i = 0; i < (unsigned int)number_of_sets(gno); i++) {
                get_graph_plotarr(gno, i, &p);
                if (is_set_active(gno, i) == TRUE) {
                    fprintf(pp, "%s    s%1d hidden %s\n", embedstr, i,
                                            true_or_false(p.hidden));
                    fprintf(pp, "%s    s%1d type %s\n", embedstr, i, set_types(p.type));
                    fprintf(pp, "%s    s%1d symbol %d\n", embedstr, i, p.sym);
                    fprintf(pp, "%s    s%1d symbol size %f\n", embedstr, i, p.symsize);
                    fprintf(pp, "%s    s%1d symbol color %d\n", embedstr, i, p.sympen.color);
                    fprintf(pp, "%s    s%1d symbol pattern %d\n", embedstr, i, p.sympen.pattern);
                    fprintf(pp, "%s    s%1d symbol fill color %d\n", embedstr, i, p.symfillpen.color);
                    fprintf(pp, "%s    s%1d symbol fill pattern %d\n", embedstr, i, p.symfillpen.pattern);
                    fprintf(pp, "%s    s%1d symbol linewidth %.1f\n", embedstr, i, p.symlinew);
                    fprintf(pp, "%s    s%1d symbol linestyle %d\n", embedstr, i, p.symlines);
                    fprintf(pp, "%s    s%1d symbol char %d\n", embedstr, i, p.symchar);
                    fprintf(pp, "%s    s%1d symbol char font %d\n", embedstr, i, get_font_mapped_id(p.charfont));
                    fprintf(pp, "%s    s%1d symbol skip %d\n", embedstr, i, p.symskip);

                    fprintf(pp, "%s    s%1d line type %d\n", embedstr, i, p.linet);
                    fprintf(pp, "%s    s%1d line linestyle %d\n", embedstr, i, p.lines);
                    fprintf(pp, "%s    s%1d line linewidth %.1f\n", embedstr, i, p.linew);
                    fprintf(pp, "%s    s%1d line color %d\n", embedstr, i, p.linepen.color);
                    fprintf(pp, "%s    s%1d line pattern %d\n", embedstr, i, p.linepen.pattern);

                    fprintf(pp, "%s    s%1d baseline type %d\n", embedstr, i, p.baseline_type);
                    fprintf(pp, "%s    s%1d baseline %s\n", embedstr, i, on_or_off(p.baseline));

                    fprintf(pp, "%s    s%1d dropline %s\n", embedstr, i, on_or_off(p.dropline));

                    fprintf(pp, "%s    s%1d fill type %d\n", embedstr, i, p.filltype);
                    fprintf(pp, "%s    s%1d fill rule %d\n", embedstr, i, p.fillrule);
                    fprintf(pp, "%s    s%1d fill color %d\n", embedstr, i, p.setfillpen.color);
                    fprintf(pp, "%s    s%1d fill pattern %d\n", embedstr, i, p.setfillpen.pattern);

                    fprintf(pp, "%s    s%1d avalue %s\n", embedstr, i, on_or_off(p.avalue.active));
                    fprintf(pp, "%s    s%1d avalue type %d\n", embedstr, i, p.avalue.type);
                    fprintf(pp, "%s    s%1d avalue char size %f\n", embedstr, i, p.avalue.size);
                    fprintf(pp, "%s    s%1d avalue font %d\n", embedstr, i, get_font_mapped_id(p.avalue.font));
                    fprintf(pp, "%s    s%1d avalue color %d\n", embedstr, i, p.avalue.color);
                    fprintf(pp, "%s    s%1d avalue rot %d\n", embedstr, i, p.avalue.angle);
                    fprintf(pp, "%s    s%1d avalue format %s\n", embedstr, i, get_format_types(p.avalue.format));
                    fprintf(pp, "%s    s%1d avalue prec %d\n", embedstr, i, p.avalue.prec);
                    fprintf(pp, "%s    s%1d avalue prepend \"%s\"\n", embedstr, i, PSTRING(p.avalue.prestr));
                    fprintf(pp, "%s    s%1d avalue append \"%s\"\n", embedstr, i, PSTRING(p.avalue.appstr));
                    fprintf(pp, "%s    s%1d avalue offset %f , %f\n", embedstr, i, p.avalue.offset.x, p.avalue.offset.y);

                    fprintf(pp, "%s    s%1d errorbar %s\n", embedstr, i, on_or_off(p.errbar.active));
                    switch (p.errbar.ptype) {
                    case PLACEMENT_NORMAL:
                        fprintf(pp, "%s    s%1d errorbar place normal\n", embedstr, i);
                        break;
                    case PLACEMENT_OPPOSITE:
                        fprintf(pp, "%s    s%1d errorbar place opposite\n", embedstr, i);
                        break;
                    case PLACEMENT_BOTH:
                        fprintf(pp, "%s    s%1d errorbar place both\n", embedstr, i);
                        break;
                    }
                    fprintf(pp, "%s    s%1d errorbar color %d\n", embedstr, i, p.errbar.pen.color);
                    fprintf(pp, "%s    s%1d errorbar pattern %d\n", embedstr, i, p.errbar.pen.pattern);
                    fprintf(pp, "%s    s%1d errorbar size %f\n", embedstr, i, p.errbar.barsize);
                    fprintf(pp, "%s    s%1d errorbar linewidth %.1f\n", embedstr, i, p.errbar.linew);
                    fprintf(pp, "%s    s%1d errorbar linestyle %d\n", embedstr, i, p.errbar.lines);
                    fprintf(pp, "%s    s%1d errorbar riser linewidth %.1f\n", embedstr, i, p.errbar.riser_linew);
                    fprintf(pp, "%s    s%1d errorbar riser linestyle %d\n", embedstr, i, p.errbar.riser_lines);
                    fprintf(pp, "%s    s%1d errorbar riser clip %s\n", embedstr, i, on_or_off(p.errbar.arrow_clip));
                    fprintf(pp, "%s    s%1d errorbar riser clip length %f\n", embedstr, i, p.errbar.cliplen);

                    if (is_hotlinked(gno, i)) {
                        fprintf(pp, "%s    s%1d link %s \"%s\"\n", embedstr, i,
                                p.hotsrc == SOURCE_DISK ? "disk" : "pipe", p.hotfile);
                    }
                    
                    fprintf(pp, "%s    s%1d comment \"%s\"\n", embedstr, i, PSTRING(p.comments));
                
                    fprintf(pp, "%s    s%1d legend  \"%s\"\n", embedstr, i, PSTRING(p.lstr));
                }
            }
        }
    }
}

static void put_objects(int gno, FILE * pp, int embed)
{
    int i;
    boxtype b;
    linetype l;
    ellipsetype e;
    plotstr s;
    char embedstr[2];
    (void)gno;

    if (embed) {
        strcpy(embedstr, "@");
    } else {
        embedstr[0] = 0;
    }
    for (i = 0; i < number_of_boxes(); i++) {
        get_graph_box(i, &b);
        if (b.active == TRUE) {
            fprintf(pp, "%swith box\n", embedstr);
            fprintf(pp, "%s    box on\n", embedstr);
            fprintf(pp, "%s    box loctype %s\n", embedstr, w_or_v(b.loctype));
            if (b.loctype == COORD_WORLD) {
                fprintf(pp, "%s    box g%1d\n", embedstr, b.gno);
            }
            fprintf(pp, "%s    box %.16g, %.16g, %.16g, %.16g\n", embedstr, b.x1, b.y1, b.x2, b.y2);
            fprintf(pp, "%s    box linestyle %d\n", embedstr, b.lines);
            fprintf(pp, "%s    box linewidth %.1f\n", embedstr, b.linew);
            fprintf(pp, "%s    box color %d\n", embedstr, b.color);
            fprintf(pp, "%s    box fill color %d\n", embedstr, b.fillcolor);
            fprintf(pp, "%s    box fill pattern %d\n", embedstr, b.fillpattern);
            fprintf(pp, "%sbox def\n", embedstr);
        }
    }

    for (i = 0; i < number_of_ellipses(); i++) {
        get_graph_ellipse(i, &e);
        if (e.active == TRUE) {
            fprintf(pp, "%swith ellipse\n", embedstr);
            fprintf(pp, "%s    ellipse on\n", embedstr);
            fprintf(pp, "%s    ellipse loctype %s\n", embedstr, w_or_v(e.loctype));
            if (e.loctype == COORD_WORLD) {
                fprintf(pp, "%s    ellipse g%1d\n", embedstr, e.gno);
            }
            fprintf(pp, "%s    ellipse %.16g, %.16g, %.16g, %.16g\n", embedstr, e.x1, e.y1, e.x2, e.y2);
            fprintf(pp, "%s    ellipse linestyle %d\n", embedstr, e.lines);
            fprintf(pp, "%s    ellipse linewidth %.1f\n", embedstr, e.linew);
            fprintf(pp, "%s    ellipse color %d\n", embedstr, e.color);
            fprintf(pp, "%s    ellipse fill color %d\n", embedstr, e.fillcolor);
            fprintf(pp, "%s    ellipse fill pattern %d\n", embedstr, e.fillpattern);
            fprintf(pp, "%sellipse def\n", embedstr);
        }
    }
   for (i = 0; i < number_of_lines(); i++) {
        get_graph_line(i, &l);
        if (l.active == TRUE) {
            fprintf(pp, "%swith line\n", embedstr);
            fprintf(pp, "%s    line on\n", embedstr);
            fprintf(pp, "%s    line loctype %s\n", embedstr, w_or_v(l.loctype));
            if (l.loctype == COORD_WORLD) {
                fprintf(pp, "%s    line g%1d\n", embedstr, l.gno);
            }
            fprintf(pp, "%s    line %.16g, %.16g, %.16g, %.16g\n", embedstr, l.x1, l.y1, l.x2, l.y2);
            fprintf(pp, "%s    line linewidth %.1f\n", embedstr, l.linew);
            fprintf(pp, "%s    line linestyle %d\n", embedstr, l.lines);
            fprintf(pp, "%s    line color %d\n", embedstr, l.color);
            fprintf(pp, "%s    line arrow %d\n", embedstr, l.arrow_end);
            fprintf(pp, "%s    line arrow type %d\n", embedstr, l.arrow.type);
            fprintf(pp, "%s    line arrow length %f\n", embedstr, l.arrow.length);
            fprintf(pp, "%s    line arrow layout %f, %f\n", embedstr, l.arrow.dL_ff, l.arrow.lL_ff);
            fprintf(pp, "%sline def\n", embedstr);
        }
    }

    for (i = 0; i < number_of_strings(); i++) {
        get_graph_string(i, &s);
        if (s.active == TRUE && s.s_plotstring[0]) {
            fprintf(pp, "%swith string\n", embedstr);
            fprintf(pp, "%s    string on\n", embedstr);
            fprintf(pp, "%s    string loctype %s\n", embedstr, w_or_v(s.loctype));
            if (s.loctype == COORD_WORLD) {
                fprintf(pp, "%s    string g%1d\n", embedstr, s.gno);
            }
            fprintf(pp, "%s    string %.16g, %.16g\n", embedstr, s.x, s.y);
            fprintf(pp, "%s    string color %d\n", embedstr, s.color);
            fprintf(pp, "%s    string rot %d\n", embedstr, s.rot);
            fprintf(pp, "%s    string font %d\n", embedstr, get_font_mapped_id(s.font));
            fprintf(pp, "%s    string just %d\n", embedstr, s.just);
            fprintf(pp, "%s    string char size %f\n", embedstr, s.charsize);
            fprintf(pp, "%s    string def \"%s\"\n", embedstr, PSTRING(s.s_plotstring));
        }
    }
}

static void put_regions(FILE * pp, int embed)
{
    int i, j;
    char embedstr[2];

    if (embed) {
        strcpy(embedstr, "@");
    } else {
        embedstr[0] = 0;
    }
    for (i = 0; i < MAXREGION; i++) {
      fprintf(pp, "%sr%1d %s\n", embedstr, i, on_or_off(rg[i].active));
      
      fprintf(pp, "%slink r%1d to g%1d\n", embedstr, i, rg[i].linkto);
      
      switch (rg[i].type) {
      case REGION_ABOVE:
	fprintf(pp, "%sr%1d type above\n", embedstr, i);
	break;
      case REGION_BELOW:
	fprintf(pp, "%sr%1d type below\n", embedstr, i);
	break;
      case REGION_TOLEFT:
	fprintf(pp, "%sr%1d type left\n", embedstr, i);
	break;
      case REGION_TORIGHT:
	fprintf(pp, "%sr%1d type right\n", embedstr, i);
	break;
      case REGION_POLYI:
	fprintf(pp, "%sr%1d type polyi\n", embedstr, i);
	break;
      case REGION_POLYO:
	fprintf(pp, "%sr%1d type polyo\n", embedstr, i);
	break;
      case REGION_HORIZI:
	fprintf(pp, "%sr%1d type horizi\n", embedstr, i);
	break;
      case REGION_VERTI:
	fprintf(pp,"%sr%1d type verti\n", embedstr, i);
	break;
      case REGION_HORIZO:
	fprintf(pp, "%sr%1d type horizo\n", embedstr, i);
	break;
      case REGION_VERTO:
	fprintf(pp,"%sr%1d type verto\n", embedstr, i);
	break;
      }
      fprintf(pp, "%sr%1d linestyle %d\n", embedstr, i, rg[i].lines);
      fprintf(pp, "%sr%1d linewidth %.1f\n", embedstr, i, rg[i].linew);
      fprintf(pp, "%sr%1d color %d\n", embedstr, i, rg[i].color);
      if (rg[i].type != REGION_POLYI && rg[i].type != REGION_POLYO) {
    fprintf(pp, "%sr%1d line %.16g, %.16g, %.16g, %.16g\n", embedstr, i, rg[i].x1, rg[i].y1, rg[i].x2, rg[i].y2);
      } else {
	if (rg[i].x != NULL) {
	  for (j = 0; j < rg[i].n; j++) {
        fprintf(pp, "%sr%1d xy %.16g, %.16g\n", embedstr, i, rg[i].x[j], rg[i].y[j]);
	  }
	}
      }
    }
}

void put_fitparms(FILE * pp, int embed)
{
    int i;
    char embedstr[2];

    if (embed) {
        strcpy(embedstr, "@");
    } else {
        embedstr[0] = 0;
    }
    
    fprintf(pp, "# Grace fit description file\n");
    fprintf(pp, "#\n");

    fprintf(pp, "%sfit title \"%s\"\n", embedstr, PSTRING(nonl_opts.title));
    fprintf(pp, "%sfit formula \"%s\"\n", embedstr, PSTRING(nonl_opts.formula));
    fprintf(pp, "%sfit with %1d parameters\n", embedstr, nonl_opts.parnum);
    fprintf(pp, "%sfit prec %g\n", embedstr, nonl_opts.tolerance);
    
    for (i = 0; i < nonl_opts.parnum; i++) {
        fprintf(pp, "%sa%1d = %g\n", embedstr, i, nonl_parms[i].value);
        if (nonl_parms[i].constr) {
            fprintf(pp, "%sa%1d constraints on\n", embedstr, i);
        } else {
            fprintf(pp, "%sa%1d constraints off\n", embedstr, i);
        }
        fprintf(pp, "%sa%1dmin = %g\n", embedstr, i, nonl_parms[i].min);
        fprintf(pp, "%sa%1dmax = %g\n", embedstr, i, nonl_parms[i].max);
    }
}

