/***************************************************************************
 *   Copyright (C) 2008-2017 by Andreas Winter                             *
 *   andreas.f.winter@web.de                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef FUNDAMENTALS_H
#define FUNDAMENTALS_H

#include <iostream>
#include <QtCore>
#include <QtGui>
#include <fstream>
#include <cstring>
#include <QMouseEvent>

#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QString>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDirModel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QColorDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinterInfo>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFontDialog>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QGraphicsProxyWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QWizard>
#include <QtWidgets/QWizardPage>
#include <QtWidgets/QWidgetAction>
#endif

#include "noxprotos.h"
#include "t1fonts.h"
#include "parser.h"
#include "graphs.h"

#define DIRLIST 0
#define FILELIST 1

#define GRAPHLIST 0
#define SETLIST 1
#define TEXTLIST 2
#define SETCHOICE 3

#define DO_NOT_PRESERVE_SELECTION 0
#define DO_PRESERVE_SELECTION 1

#define DELTASIZE 32

#define TREE_ROLE_TYPE 33
#define TREE_ROLE_GRAPH 34
#define TREE_ROLE_SET 35

#define SLIDE_LINEAR 0
#define SLIDE_LOGARITHMIC 1

#define ANSWER_YES 1
#define ANSWER_NO 0
#define ANSWER_SAVE -1

using namespace std;

void append_to_storage(int * len,int ** storage,int n,int * new_entries);
void append_to_storage2(int * l,int ** storage_a,int ** storage_b,int n,int * new_entries_a,int * new_entries_b);
int indexOfFontInDatabase(QFont & f);
QFont getFontFromDatabase(int i);
int addFontToDatabase(QFont & f);
void appendTextToLegendString(int gno,int sno,QString text);
void getSetIDFromText(char * text,int & gno,int & sno,int & column);

void DynSetMemoryToLineEdit(char * &t1,char * &t2,char *&c1,char * &c2,bool & displayStd,bool acceptLaTex,QLineEdit * lenText);
void SetLineEditToMemory(char * t1,char * t2,char * &c1,char * &c2,bool &displayStd,QLineEdit * lenText);

void delete_special_int(int ** target);//deletes something and sets it to NULL afterwards
void resize_special_int(int ** target,int o_size,int n_size);//resizes an array, but keeps the old contents

struct komplex
{
double real;
double imag;
};

struct komplex add_komplex(struct komplex a,struct komplex b);
struct komplex sub_komplex(struct komplex a,struct komplex b);
struct komplex mult_komplex(struct komplex a,struct komplex b);
struct komplex div_komplex(struct komplex a,struct komplex b);
struct komplex pow_komplex(struct komplex a,double n);

//invalid
#define ID_NONE -1
//G0.S1
#define ID_REGULAR_COMPLETE 0
//G1N.S4
#define ID_NGSS 1
//G2.S1N
#define ID_SGNS 2
//G1N.S3N
#define ID_NGNS 3
//S0
#define ID_SET_ONLY 4
//S3N
#define ID_NSET_ONLY 5
//G0
#define ID_GRAPH_ONLY 6
//G1N
#define ID_NGRAPH_ONLY 7
//Y
#define ID_COLUMN_ONLY 8
//G#
#define ID_GRAPH_NR_ONLY 9
//S#
#define ID_SET_NR_ONLY 10
//#QTGRACE_SPECIAL
#define ID_SPECIAL_COMMAND 11

//column only is the position of [X,Y,Y1,Y2,Y3,Y4]
int searchTextForFirstID(QString text,int start,int & pos,int & g_no,int & s_no,char & control1,char & control2,char & col,int & leng);
QString createReplacementID(int type,int n_gno,int n_sno,int o_gno,int o_sno,char control1,char control2,char col);
QString createCompleteID(int type, int n_gno, int n_sno, int o_gno, int o_sno, char control1, char control2, char col);

class single_formula_token
{
public:
single_formula_token(char * token=NULL);
~single_formula_token();
int type;//type=0 --> an integer counter, type=1 --> a double value, type=2 --> unknown
//counters have to be increased manually
//double values have to be increased manually (usually by evaluating a formula)
char * representation;//the 'name' of the token (necessary to replace it in a given formula)
private:
int i_counter;//the internal counter-value
double d_value;//the double value
public:
void initialize(void * value,int n_type=-1);//initialize the token (it is possible to change the type here, -1 means: no change) - this means: set the value
void changeCounter(int delta=1);//increases the i_counter-value by delta
void setValueToFormula(char * formula=NULL);//evaluates a formula and sets the d_value to the result (NULL does not change anything)
int get_counter_value(void);
void raise_counter(void);
void lower_counter(void);
double get_d_value(void);
void reset_token(char * token);//sets the token to a new value (does not reset the counter and double value)
class single_formula_token & operator=(class single_formula_token & a);
};

class uniList;

class GraphPopup:public QMenu
{
Q_OBJECT
public:
int selected_no;
int number_of_selected_graphs;
int * selected_graphs;

uniList * par;

GraphPopup(uniList *parent=0);
~GraphPopup();

QAction *actLabel,*actFocusTo,*actShow,*actHide,*actDuplicate,*actKill,*actSwap,*actCreateNew;
QAction *actCopy12,*actCopy21,*actMove12,*actMove21;

public slots:
void CreateActions(void);
void doNothing(void);
void doFocusTo(void);
void doShow(void);
void doHide(void);
void doDuplicate(void);
void doKill(void);
void doSwap(void);
void doCreateNew(void);
void doMove12(void);
void doMove21(void);
void doCopy12(void);
void doCopy21(void);
void update(void);
void update_menu_content(void);
};

class SetPopup:public QMenu
{
Q_OBJECT
public:
int selected_graph;//graph the selected sets belong to
int number_of_selected_sets;
int * selected_sets;

uniList * par;
SetPopup(uniList *parent=0);
~SetPopup();

QMenu * mnuEdit;
QMenu * mnuCreateNew;
QMenu * mnuDisplay;
QMenu * mnuColor,*mnuLineStyle,*mnuSymbol;
QMenu * mnuSelectorOperations;
QMenu * mnuSimpleOperations;
QAction * actLabel;
QAction * actHide,*actBringToFront,*actSendToBack,*actPackAllSets;
QAction * actShow,*actDuplicate,*actKill,*actKillData,*actSwap;
QAction * actCreateByFormula,* actCreateInSpreadsheet,* actCreateInTextEditor,* actCreateFromBlockData;
QAction * actEditInSpreadsheet,* actEditInTextEditor;
QAction * actViewSetComments,*actViewSetLegends,*actViewIcons,*actShowDataLess,*actShowHidden,*actSelectAll,*actUnselectAll,*actInvertSelection,*actUpdate;
QAction * actCopy12,*actCopy21,*actMove12,*actMove21;
QAction * actCopyClipBoard,*actPasteClipBoard;
QAction * actSelectEven,*actSelectOdd,*actSelectVisible,*actSelectInvisible,*actSelectNth;
QAction * actStoreAppearance,*actApplyStoredAppearance;
QAction * actSimpleSortXUp,*actSimpleSortXDown,*actSimpleReverse,*actSimpleJoin,*actSimpleRestrict,*actSimpleSwapXY;
QAction ** actColors,**actLineStyles,**actSymbols;
int nr_of_col_items,nr_of_linestyles,nr_of_symbols;
int * maincolors;
QSignalMapper * colMapper;
QSignalMapper * lineStyleMapper;
QSignalMapper * symbolMapper;
//QAction * actSyncListContents;

public slots:
void prepareForAction(void);
void finishAction(int preserveSelection);
void CreateActions(void);
void doNothing(void);
void doHide(void);
void doShow(void);
void doSendToBack(void);
void doBringToFront(void);
void doDuplicate(void);
void doKill(void);
void doKillData(void);
void doSwap(void);
void doPackAllSets(void);
void doEditInSpreadSheet(void);
void doEditInTextEditor(void);
void doCreateByFormula(void);
void doCreateInSpreadsheet(void);
void doCreateInTextEditor(void);
void doCreateFromBlockData(void);
void doSelectAll(void);
void doUnSelectAll(void);
void doSelectEven(void);
void doSelectOdd(void);
void doSelectVisible(void);
void doSelectInVisible(void);
void doSelectNth(void);
void doUpdate(void);
void doInvertSelection(void);
void doShowHidden(void);
void doShowDataLess(void);
void doViewSetComments(void);
void doViewSetLegends(void);
void doViewIcons(void);
void doMove12(void);
void doMove21(void);
void doCopy12(void);
void doCopy21(void);
void doCopyClipBoard(void);
void doPasteClipBoard(void);
void doStoreAppearance(void);
void doApplyStoredAppearance(void);
void doSimpleSortXUp(void);
void doSimpleSortXDown(void);
void doSimpleReverse(void);
void doSimpleJoin(void);
void doSimpleRestrict(void);
void doSimpleSwapXY(void);
void generateColorMenu(void);
void generateLinesMenu(void);
void generateSymbolsMenu(void);
void colorClicked(int c);
void lineStyleClicked(int c);
void symbolsClicked(int c);
//void doSyncList(void);
void update_menu_content(void);
virtual void hideEvent(QHideEvent * e);
};

class uniList:public QListWidget
{
Q_OBJECT
public:
int datType;
int number_of_entries;
int gr_no;
int * entries;//set or graph numbers (because hidden numbers are not displayed --> item-number != set-number)
QString * text_entries;//only used in TEXTLIST
bool minimum_display;//reduces the data display to absolute minimum
bool show_hidden;
bool show_data_less;
bool show_comments,show_legends,show_icons;
bool prevent_from_autoupdate;
bool deselect_possible;
bool show_all_sets_marker,all_entries_option_selected;
//bool allowSnycCommand;

class uniList * partner;

SetPopup * popupMenu1;
GraphPopup * popupMenu2;

QAction *actCopyClipBoard,*actPasteClipBoard;

uniList(int type,QWidget *parent=0);
~uniList();

public slots:
void update_number_of_entries(void);//reread and display entries - selection is cleared
void update_number_of_entries_preserve_selection(void);//same as above, selection is not cleared
void get_selection(int * number,int ** selection);
void get_selection(int * number,QString ** selection);//only useful for TEXTLIST
void clear_text_items(void);
void add_Item(QString item);
void set_graph_number(int gno,bool keep_selection);
void new_selection(void);
void set_new_selection(int number,int * selection);
void entryDoubleClicked(QListWidgetItem * item);
void activateHighlighting(void);
virtual void mousePressEvent(QMouseEvent * e);
virtual void mouseReleaseEvent(QMouseEvent * e );
virtual void keyPressEvent(QKeyEvent * e);
virtual void keyReleaseEvent(QKeyEvent * e);
virtual void focusOutEvent(QFocusEvent *event);
void setBehavior(bool deselect,bool multi,bool all_sets);
void doCopyClipBoard(void);
void doPasteClipBoard(void);
//void doSyncNow(void);
signals:
void new_selection(int a);
//void syncSelection(void);
friend class SetPopup;
};

class SetCombo:public QComboBox
{
Q_OBJECT
public:
SetCombo(QWidget * parent=0);
bool show_none;
int gno;
int nr_of_entries;
int * snos;
public slots:
void update_entries(int gno,bool preserve_selection);
int value(void);
void setValue(int set);
};

class SetSelectorCombo:public QWidget
{
Q_OBJECT
public:
SetSelectorCombo(QString text,QWidget * parent=0);
SetCombo * cmb;
QLabel * lblCombo;
QHBoxLayout * layout;
public slots:
void update_entries(int gno,bool preserve_selection);
int value(void);
void setValue(int set);
};

class Panel:public QLabel
{
Q_OBJECT
public:
Panel(QPixmap * pix,QWidget * parent=0);
QPixmap * p;
bool inpaint;
public slots:
void mouseMoveEvent( QMouseEvent * event );
void mouseReleaseEvent(QMouseEvent *event);
void paintEvent(QPaintEvent *event);
signals:
void mouseMoving(void);
void mouseClicked(void);
};

class mySlider:public QSlider
{
Q_OBJECT
public:
mySlider(Qt::Orientation orientation, QWidget *parent = NULL);
public slots:
virtual void focusOutEvent(QFocusEvent * event);
signals:
void focusOut(void);
};

class verticalSliderPopup:public QWidget
{
Q_OBJECT
public:
verticalSliderPopup(int width,QWidget * parent=0);
//QSlider * slider;
mySlider * slider;
int width_hint;
QTime * visible_timer;
public slots:
void setRange(int min_v,int max_v);
virtual void showEvent(QShowEvent * event);
virtual void focusOutEvent(QFocusEvent * event);
void focusOut(void);
};

class alphaFrame:public QFrame
{
Q_OBJECT
public:
alphaFrame(QWidget * parent=0,bool hasSlider=true);
verticalSliderPopup * slider;
int alpha;
public slots:
virtual void mousePressEvent( QMouseEvent * event );
virtual void mouseReleaseEvent( QMouseEvent * event );
virtual void paintEvent(QPaintEvent * event);
virtual void resizeEvent( QResizeEvent * event );
virtual QSize sizeHint() const;
void setAlpha(int a);
void setAlpha_extern(int a);
signals:
void valueChanged(int i);
};

class horizontalAlphaSlider:public QWidget
{
Q_OBJECT
public:
horizontalAlphaSlider(QWidget * parent=0);
QHBoxLayout * layout;
alphaFrame * lblAlpha;
QSlider * slider;
QLabel * lblSlider;
public slots:
void setAlpha(int a);
void setAlpha_extern(int a);
virtual void mousePressEvent( QMouseEvent * event );
virtual void mouseReleaseEvent( QMouseEvent * event );
virtual void leaveEvent( QEvent * event );
signals:
void alphaChanged(int i);
};

class panelWindow:public QWidget//QDialog
{
Q_OBJECT
public:
panelWindow(int rows,int cols,int last_col,QPixmap ** pix,QWidget * parent=0,bool hasAlphaSlider=false);
~panelWindow();
class Panel ** lblPanel;
horizontalAlphaSlider * alphaSlider;
QTime * clickTime;
QGridLayout * layout;
QHBoxLayout * layout0;
QFrame * frm;
QPixmap ** pixmaps;
QSignalMapper * signalMapper1;
QSignalMapper * signalMapper2;
int row_count,col_count,last_col_count,number_of_elements;
int marked;//the always-raised-element
int highlighted;//the sunken element while mouse is over it
int elementWidth,elementHeight;
int pressed;
int cur_element;
int movex,movey;
int nr_of_index_entries;
int alpha;
int * index_entries;//the numbers associated with a panel (usually the index itself, but not always)
public slots:
void setAlpha_extern(int a);
void setMarked(int i);//set the current panel
void mouseMovedIn(int i);//MouseMoved in one of the Panels
void mouseClickedIn(int i);//Users clicked on one of the Panels
virtual void leaveEvent( QEvent * event );
virtual void mouseReleaseEvent(QMouseEvent *event);
virtual void mousePressEvent(QMouseEvent *event);
virtual void mouseMoveEvent(QMouseEvent * event);
virtual void keyPressEvent(QKeyEvent * event);
virtual void focusOutEvent(QFocusEvent * event);
int element(int x,int y);//return the number of the element at position x,y (-1 if there is no element)
signals:
void newSelection(int i);
void newAlpha(int a);
};

class newCombo:public QComboBox
{
Q_OBJECT
public:
newCombo(int rows,int cols,int last_col,QPixmap ** pix,QString ** titles,bool showTextOnly=true,QWidget * parent=0,bool hasAlphaSlider=false);
void reinitializePanels(int rows,int cols,int last_col,QPixmap ** pix,QString ** titles,bool showTextOnly=true,bool hasAlphaSlider=false);
panelWindow * panels;
QIcon * simpleColorIcons;
QString * title_strings;
int selection;
bool text_only;
public slots:
void createSimpleColorIcons(void);
void setCurrentIndex(int index);
int currentIndex(void);
virtual void showPopup();
virtual void wheelEvent(QWheelEvent * e);
private slots:
void wrapperSetCurrentIndex(int index);
signals:
void current_Index_Changed( int index );
};

class fitLine:public QWidget
{
Q_OBJECT
public:
fitLine(QWidget * parent=0,int nr=0);

QLabel * lblA;
QLabel * lblABounds;
QCheckBox * chkBonds;
QLineEdit * ledValue;
QLineEdit * ledLowerBound;
QLineEdit * ledHighterBound;
QHBoxLayout * layout;
public slots:
void getValues(double & value,bool & active,double & lowerBound,double & upperBound);
void constr_check(int t);
void Redisplay(void);
};

class axisLine:public QWidget
{
Q_OBJECT
public:
axisLine(QWidget * parent=0,int nr=0);
bool original;
char * c1,*c2;
QLabel * lblNr;
QLineEdit * ledLocation;
QLineEdit * ledLabel;
QHBoxLayout * layout;
public slots:
QString LabelText(void);
QString LocationText(void);
void SetTextToMemory(char * &t1,char * &t2);
void SetMemoryToText(char * &t1,char * &t2);
void LabelClicked(void);
void mouseReleaseEvent(QMouseEvent * e);
};

class headLine:public QWidget
{
Q_OBJECT
public:
headLine(QWidget * parent=0,int columns=0);
int cols;
QPushButton * cmdColHead[6];
QHBoxLayout * layout;
QWidget * empty;
QSignalMapper * signalMapper;
public slots:
void pressed(int i);
void resize(int columns);
signals:
void col_pressed(int i);
};

class spreadLine:public QWidget
{
Q_OBJECT
public:
spreadLine(QWidget * parent=0,int row_nr=0,int columns=0);
~spreadLine();
bool marked;
int row;
int cols;
QPushButton * cmdSelect;
QLineEdit * ledInput[6];
QHBoxLayout * layout;
public slots:
void clickSelect(void);
void resize(int columns);
};

class spreadSheet:public QWidget
{
Q_OBJECT
public:
spreadSheet(QWidget * parent,int type,int row_count,int col_count);
~spreadSheet();
int rows;//rows with actual inputs
int cols;
int sheet_type;//type=0-->spreadsheet, type=1-->fit-spreadsheet, type=2-->axis-properties
int stdWidth,stdHeight;

axisLine ** axislines;
fitLine ** fitlines;
headLine * headline;
spreadLine ** spreadlines;

QWidget * empty;
QSignalMapper * signalMapper;
QVBoxLayout * layout;
public slots:
void clickHHeader(int nr);
void clickVHeader(int nr);
void resize(int row,int col);
void generate_layout(void);
void entryChanged(void);
signals:
void changed(void);
};

class stdSlider:public QWidget
{
Q_OBJECT
public:
stdSlider(QWidget * parent,QString label,int min,int max,double factor=1.0,int type=SLIDE_LINEAR);
//QLabel * Indicator;
QLineEdit * Indicator;
QLabel * lblText;
QSlider * sldSlider;
QTime * ret_time;
int textHeight,slideType;
int old_value;//important for immediate updates and undo-list
double ScalingFactor;
virtual void resizeEvent( QResizeEvent * e);
signals:
void valueChanged(int i);
public slots:
virtual QSize sizeHint() const;
virtual QSize minimumSizeHint() const;
void changed(int i);
void SliderMoved(int i);
void SliderReleased(void);
void SliderPressed(void);
int value(void);
void setValue(int i);
void redisplay(void);
QString indicatorText(void);
void slideIndicator(void);
void Indicator_Enter_Pressed(void);
void Indicator_Finished(void);
};

class FontSelector:public QWidget
{
Q_OBJECT
public:
FontSelector(QWidget * parent=0);

QLabel * lblText;//just a label
QComboBox * cmbFontSelect;//standard-combo-box for selecting one of a few predefined T1-fonts (or preselected Qt-fonts)
QPushButton * cmdSelFont;//a button to open the font selector dialog of the operating system --> only for Qt-fonts
QFont font;//currently selected Qt-font
QHBoxLayout * layout;
signals:
void currentIndexChanged(int i);
public slots:
void updateAppearance(bool QtIsNew);//updates the current appearance of the selector according to the setting of useQtFonts
void setLabelText(QString s);
void updateFonts(bool preserve);//displays all fonts that are currently available in the font-selector
void displayFont(void);//shows how the font looks like in the command button
void selectNewFont(void);
QFont getCurrentFont(void);
void setCurrentFont(QFont & f);
void changed(int i);
int currentIndex(void);
void setCurrentIndex(int i);
};

class PositionSelector:public QWidget
{
Q_OBJECT
public:
PositionSelector(QWidget * parent=0);

QLabel * lblText;
QComboBox * cmbPositionSelect;
QHBoxLayout * layout;

public slots:
int currentIndex(void);
int currentValue(void);
void setCurrentIndex(int i);
};

class JustificationSelector:public QWidget
{
Q_OBJECT
public:
JustificationSelector(QWidget * parent=0);
int values[12];
QLabel * lblText;
newCombo * cmbJustSelect;
QHBoxLayout * layout;
QString ** just_names;
public slots:
int currentIndex(void);
void setCurrentIndex(int i);
};

class ColorSelector:public QWidget
{
Q_OBJECT
public:
ColorSelector(QWidget * parent=0);

QLabel * lblText;
newCombo * cmbColorSelect;
alphaFrame * alphaSelector;
QHBoxLayout * layout;
bool prevent_from_update;

public slots:
void update_color_panels(void);//delete all color panels and create new ones according to current color palette
int currentIndex(void);
int alpha(void);
void setCurrentIndex(int i);
void setAlpha(int i);
void updateColorIcons(int nr_of_cols,QPixmap ** ColorPixmaps,QString ** ColorNames);
//private slots:
void panelIndexChanged(int i);
void alphaValueChanged(int i);
void alphaValueChanged2(int i);
signals:
void currentIndexChanged(int i);
void alphaChanged(int i);
};

class StdSelector:public QWidget
{
Q_OBJECT
public:
StdSelector(QWidget * parent=0);
StdSelector(QWidget * parent,QString label,int number,QString * entr);
~StdSelector();

int number_of_entries;
int * entryValues;
QString * entries;
QIcon * entryIcons;

QLabel * lblText;
QComboBox * cmbSelect;
QHBoxLayout * layout;
signals:
void currentIndexChanged(int i);
public slots:
void changed(int i);
void setValues(int * values);
void setNewEntries(int number,QString * entr);
void setNewEntries(int number,QString * entr,int * values);
void setIcons(QIcon * icons=NULL);
void redisplayItems(void);
int currentIndex(void);
int currentValue(void);
void setCurrentIndex(int i);
void setCurrentValue(int i);
int indexOfValueInList(int v);//returns the index of the value v in the entryValues-list (-1 if not found)
bool valueIsInList(int v);//returns true, if v is in the list of entryValues
};

class LineStyleSelector:public QWidget
{
Q_OBJECT
public:
LineStyleSelector(QWidget * parent=0);

QString ** LineNames;
QLabel * lblText;
//QComboBox * cmbStyleSelect;
newCombo * cmbStyleSelect;
QHBoxLayout * layout;
signals:
void currentIndexChanged(int i);
public slots:
void changed(int i);
int currentIndex(void);
void setCurrentIndex(int i);
};

class FillPatternSelector:public QWidget
{
Q_OBJECT
public:
FillPatternSelector(QWidget * parent=0);

QString ** PatternNames;
QLabel * lblText;
//QComboBox * cmbFillPattern;
newCombo * cmbFillPattern;
QHBoxLayout * layout;
signals:
void currentIndexChanged(int i);
public slots:
void changed(int i);
int currentIndex(void);
void setCurrentIndex(int i);
};

class OrderSelector:public QWidget
{
Q_OBJECT
public:
OrderSelector(QWidget * parent=0);

QString ** OrderNames;
QLabel * lblText;
newCombo * cmbOrderPattern;
QHBoxLayout * layout;
signals:
void currentIndexChanged(int i);
public slots:
void changed(int i);
int currentIndex(void);
void setCurrentIndex(int i);
};

class LineWidthSelector:public QWidget
{
Q_OBJECT
public:
LineWidthSelector(QWidget * parent=0);
QLabel * lblText;
QDoubleSpinBox * spnLineWidth;
QHBoxLayout * layout;
signals:
void currentValueChanged(double i);
public slots:
void setText(QString txt);
void setRange(double a,double b);
void setSingleStep(double a);
void setDecimals(int d);
void changed(double i);
double value(void);
void setValue(double i);
};

class stdIntSelector:public QWidget
{
Q_OBJECT
public:
stdIntSelector(QWidget * parent,QString label,int min,int max);
QLabel * lblText;
QSpinBox * spnInt;
QHBoxLayout * layout;
signals:
void currentValueChanged(int i);
public slots:
void changed(int i);
int value(void);
void setValue(int i);
};

class stdLineEdit:public QWidget
{
Q_OBJECT
public:
stdLineEdit(QWidget * parent,QString label,bool accLaTex=false);
bool acceptLaTex;//controls whether this LineEdit allows LaTeX-input
bool displayStd;//Std is the original user input (probably in LaTeX-format)
QLabel * lblText;
QLineEdit * lenText;
QHBoxLayout * layout;
char * c1, * c2;//places where the text is stored at (Grace- and LaTeX-/Original-version)
signals:
void changed(void);
void NoMoreValidAdr(void);
public slots:
void ContentChanged(void);
void clickedOnLabel(void);
void mouseReleaseEvent(QMouseEvent * e);
//the following two functions are for standard-text-operations without LaTeX-support
QString text(void);
void setText(QString text);
//The following two functions are only for use with static arrays
void SetTextToMemory(char * t1,char * t2);//Function to set text according to t2 in the memory
void SetMemoryToText(char * t1,char * t2);//Function to get the text and place it in the memory in t2 and convert t2 form LaTeX to Grace and set it to t1
//The following function is only for use with dynamic arrays --> copy_string
void DynSetMemoryToText(char * &t1,char * &t2);
void ReplaceNumberContents(void);//replaces the decimal separators according to the setting of 'OldDecimalPoint' and 'DecimalPointToUse' --> should only be used on lines where numbers and formulas are to be entered
void RedisplayContents(void);//refreshes the displayed text (usually after a change in the underlying addresses)
void setDoubleValue(double val);//same as below, but uses the default double-value-format (sformat, usually "%.8g")
void setDoubleValue(const char * form,double val);
double getDoubleValue(void);
int getIntValue(void);
double guessDoubleValue(void);//tries to convert the text entered into double-value (this should also work for integer) -- a lot of guesswork here -- it should work if the user is not trying to be funny (in which case he/she will get funny results)
virtual void dropEvent(QDropEvent *);
virtual void dragEnterEvent(QDragEnterEvent *event);
};

class stdButtonGroup:public QWidget
{
Q_OBJECT
public:
stdButtonGroup(QWidget * parent=0,bool appl=true,bool acc=true,bool help=false);

bool showAccept;
bool showApply;
bool showHelp;

QPushButton * cmdAccept;
QPushButton * cmdApply;
QPushButton * cmdClose;
QPushButton * cmdHelp;
QHBoxLayout * layout;
public slots:

};

class dirList:public QTreeView
{
Q_OBJECT
public:
dirList(QWidget * parent=0,int type=DIRLIST);
int listtype;
QDirModel *model;

public slots:

};

class FileSelector:public QWidget
{
Q_OBJECT
public:
FileSelector(QWidget * parent=0);

QLabel * lblFilter;
QLabel * lblDirs;
QLabel * lblFiles;
stdLineEdit * ledFilter;
QListView * DirList;
QListView * FileList;
QCheckBox * chkShowHiddenFiles;
QGroupBox * grpChDir;
QHBoxLayout * layout0;
StdSelector * selChdir;
QPushButton * cmdSetCwd;
//QPushButton * cmdGetCwd;
QPushButton * cmdGoUp;
QPushButton * cmdStdDialog;

QDirModel *modelDirs;
QDirModel *modelFiles;

QString filter;
QString filterExtension;

bool forRead;//select only existing files
bool doubleclickfile;
bool doubleclickdir;
#ifdef WINDOWS_SYSTEM
bool onHighestLevel;//for non-Unix-Systems
#endif
QString separator;
QString currentDir;
QString selectedFile;
QString selectedDir;
QString cur_FileName;

QGridLayout * layout;
public slots:
void GeneratePathWithExtension(QDir dir,QString & PathAndExtension,QString & DirectoryOnly);
void doGoUp(void);//Go upwards one directory
void doStdDialog(void);//Open the OS standard dialog for file opening / saving
void FileSelected(const QModelIndex & index );
void FileDoubleClicked( const QModelIndex & index );
void DirSelected(const QModelIndex & index );
void DirDoubleClicked( const QModelIndex & index );
void showFilesLikeFilter(void);
void newFilterEntered(void);
void doSetCWD(void);
//void doGetCWD(void);
void toggleHiddenFile(int i);
void setFilterFromExtern(QString & directory,QString & extension);
void setFileSelectionFromExtern(QString n_file);
void currentDirChanged(int i);
signals:
void newSelectionManually(void);
void newSelection(QString selection);
void newSelectionDoubleClick(QString selection);
};

class stdStartStop:public QWidget
{
Q_OBJECT
public:
stdStartStop(QWidget * parent=0);

QLabel * lblStart;
QLabel * lblStop;
QLabel * lblLength;
QLineEdit * ledStart;
QLineEdit * ledStop;
QLineEdit * ledLength;

QHBoxLayout * layout;

bool error;

public slots:
void Redisplay(void);//changes the decimal separators
double start(void);
double stop(void);
int length(void);
};

class stdSetTypeSelector:public StdSelector
{
Q_OBJECT
public:
stdSetTypeSelector(QWidget * parent=0);
int column_count;
bool showAll;
public slots:
void setShowAll(bool sa);
void setColumnCount(int count);
int getColumnCount(void);
};

void construct_string_from_data(double value,char * target,int col_precision,int col_format);

class SetTableModel:public QAbstractTableModel
{
Q_OBJECT
private:
int * col_format;
int * col_precision;
public:
int setnr,graphnr;
SetTableModel(int gno=-1,int sno=-1,int * colformat=NULL,int * colprecision=NULL,QObject * parent=0);
bool init(int gno,int sno);
int rowCount(const QModelIndex & parent = QModelIndex()) const;
int columnCount(const QModelIndex & parent = QModelIndex()) const;
QVariant data(const QModelIndex & index,int role = Qt::DisplayRole) const;
bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex() );
bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex() );
bool insertColumns(int column, int count, const QModelIndex & parent = QModelIndex() );
bool removeColumns(int column, int count, const QModelIndex & parent = QModelIndex() );
virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
void major_changes(void);
public slots:

signals:
void item_entry_changed(int row,int col,bool really_new_value);
};

class SetTableView:public QTableView
{
Q_OBJECT
public:
SetTableView(int gno=-1,int sno=-1,int * colformat=NULL,int * colprecision=NULL,QWidget * parent=0);
SetTableModel * model;
public slots:
bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);
void dataChanged(const QModelIndex &topLeft,const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
};

/*int GetSpinChoice(stdIntSelector * s);
double GetSpinChoice(LineWidthSelector * s);
double GetSpinChoice(QDoubleSpinBox * s);    
bool GetToggleButtonState(QCheckBox * c);*/

class treeView;
class frmExplorer;

class TreePopup:public QMenu
{
Q_OBJECT
public:
char selected_type;
int gno,sno;
bool possible;
treeView * par;
frmExplorer * par2;
TreePopup(treeView *parent=0);
~TreePopup();
QAction *actShow,*actHide,*actKill,*actDuplicate;
public slots:
void CreateActions(void);
void doNothing(void);
void doShow(void);
void doHide(void);
void doKill(void);
void doAction(int type);
void doDuplicate(void);
void updateEntries(char ty,int g,int s);
};

class treeView:public QTreeWidget
{
Q_OBJECT
public:
treeView(QWidget * parent=0);
TreePopup * popup;
public slots:
void NameProject(void);//fill the header with usefull informations
void AddMajorChild(QTreeWidgetItem * parent,QTreeWidgetItem * item);
void AddMinorChild(QTreeWidgetItem * parent,QTreeWidgetItem * item);
void ClearAll(void);
void RecreateCompleteTree(void);
void itemClickedAt(QTreeWidgetItem * item,int col);
void getItemData(QTreeWidgetItem * item,char & type,int & gno,int & sno);
QTreeWidgetItem * findItem(char type,int gno,int sno);
void mousePressEvent(QMouseEvent * e);
void selectItem(char type,int gno,int sno);
signals:
void newItemClicked(char type,int gno,int sno);
};

class InLineEditor:public QLineEdit
{
Q_OBJECT
public:
InLineEditor(QWidget * parent=0);
int edit_type;//0=text;1=title;2=axis-label;3=legend;4=custom tick
int id1,id2,id3,id4;
//edit_type=0 --> id1=graph_nr, id2=text-id
//edit_type=1 --> id1=graph_nr, id2=(0=title,1=sub_title)
//edit_type=2 --> id1=graph_nr, id2=axis-nr(x,y,alt-x,alt-y); id4=0=normal, =1=opposite
//edit_type=3 --> id1=graph_nr, id2=set_nr
//edit_type=4 --> id1=graph_nr, id2=axis-nr(x,y,alt-x,alt-y), id3=tick-nr
int old_active_value;
char * old_text;
QRect bounding_box;
QPalette pal;
public slots:
void init_editor(int type,int id_1,int id_2,int id_3=0,int id_4=0);
void show_editor(void);
void hide_editor(bool redraw);
void finish_editor(void);
virtual void focusOutEvent(QFocusEvent * event);
virtual void keyPressEvent(QKeyEvent * event);
signals:
void editFinished(void);
void editorClosed(void);
};

class InLineEditor2:public QGraphicsView
{
Q_OBJECT
public:
InLineEditor2(QWidget * parent=0);
InLineEditor * lenEdit;
QGraphicsScene * scene;
QGraphicsProxyWidget *proxy;
QPalette transpPalette;
public slots:
void init_editor(int type,int id_1,int id_2,int id_3=0,int id_4=0);
void show_editor(double angle);
void hide_editor(void);
void finish_editor(void);
void setRotationAngle(double angle);
void resetRotation(void);
void getTextOffset(int & x_shift,int & y_shift);
void init_and_show_editor(int type,int id_1,int id_2,int id_3=0,int id_4=0);
signals:
void editFinished(void);
};

class SpinBoxAction:public QWidgetAction
{
Q_OBJECT
public:
SpinBoxAction(const QString & title1,const QString & title2,QWidget * parent);
QWidget * empty;
QSpinBox * spnValue;
QLabel * lblText1;
QLabel * lblText2;
QHBoxLayout * layout;
};

#define NUMBER_OF_COLUMN_FORMATS 13

#define COLUMN_OFFSET 0
#define COLUMN_SIG_CHAR 1
#define COLUMN_USIG_CHAR 2
#define COLUMN_SIG_SHORT 3
#define COLUMN_USIG_SHORT 4
#define COLUMN_SIG_INT 5
#define COLUMN_USIG_INT 6
#define COLUMN_SIG_LONG 7
#define COLUMN_USIG_LONG 8
#define COLUMN_FLOAT 9
#define COLUMN_DOUBLE 10
#define COLUMN_LONG_DOUBLE 11
#define COLUMN_STRING 12

//#define NUMBER_OF_IMPORT_DESTINATIONS 61
#define NUMBER_OF_IMPORT_DESTINATIONS 35

#define IMPORT_TO_NONE 0
#define IMPORT_TO_TITLE 1
#define IMPORT_TO_X 2
#define IMPORT_TO_Y 3
#define IMPORT_TO_Y1 4
#define IMPORT_TO_Y2 5
#define IMPORT_TO_Y3 6
#define IMPORT_TO_Y4 7
#define IMPORT_TO_X0 8
#define IMPORT_TO_DELTAX 9
#define IMPORT_TO_XTITLE 10
#define IMPORT_TO_YTITLE 11
#define IMPORT_TO_XFACTOR 12
#define IMPORT_TO_YFACTOR 13
#define IMPORT_TO_Y1FACTOR 14
#define IMPORT_TO_Y2FACTOR 15
#define IMPORT_TO_Y3FACTOR 16
#define IMPORT_TO_Y4FACTOR 17
#define IMPORT_TO_SUBTITLE 18
#define IMPORT_TO_NUMBER_OF_DATA 19
#define IMPORT_TO_NUMBER_OF_CHANNELS 20
#define IMPORT_TO_DATA_SIZE 21
#define IMPORT_TO_DATA_SIZE_BIT 22
#define IMPORT_TO_DATA_SAMPLING_RATE 23
#define IMPORT_TO_WHOLE_DATA_BLOCK_SIZE 24
#define IMPORT_TO_SINGLE_DATA_BLOCK_SIZE 25
#define IMPORT_TO_SET_LEGEND 26
#define IMPORT_TO_TRIGGER 27
#define IMPORT_TO_X_OFFSET 28
#define IMPORT_TO_Y_OFFSET 29
#define IMPORT_TO_Y1_OFFSET 30
#define IMPORT_TO_Y2_OFFSET 31
#define IMPORT_TO_Y3_OFFSET 32
#define IMPORT_TO_Y4_OFFSET 33
#define IMPORT_TO_DATA_START_OFFSET 34

/*
#define IMPORT_TO_TRIGGER_FACTOR 28
#define IMPORT_TO_CHANNEL0_FACTOR 29
#define IMPORT_TO_CHANNEL1_FACTOR 30
#define IMPORT_TO_CHANNEL2_FACTOR 31
#define IMPORT_TO_CHANNEL3_FACTOR 32
#define IMPORT_TO_CHANNEL4_FACTOR 33
#define IMPORT_TO_CHANNEL5_FACTOR 34
#define IMPORT_TO_CHANNEL6_FACTOR 35
#define IMPORT_TO_CHANNEL7_FACTOR 36
#define IMPORT_TO_CHANNEL8_FACTOR 37
#define IMPORT_TO_CHANNEL9_FACTOR 38
#define IMPORT_TO_CHANNEL10_FACTOR 39
#define IMPORT_TO_CHANNEL11_FACTOR 40
#define IMPORT_TO_CHANNEL12_FACTOR 41
#define IMPORT_TO_CHANNEL13_FACTOR 42
#define IMPORT_TO_CHANNEL14_FACTOR 43
#define IMPORT_TO_CHANNEL15_FACTOR 44
#define IMPORT_TO_CHANNEL0_OFFSET 45
#define IMPORT_TO_CHANNEL1_OFFSET 46
#define IMPORT_TO_CHANNEL2_OFFSET 47
#define IMPORT_TO_CHANNEL3_OFFSET 48
#define IMPORT_TO_CHANNEL4_OFFSET 49
#define IMPORT_TO_CHANNEL5_OFFSET 50
#define IMPORT_TO_CHANNEL6_OFFSET 51
#define IMPORT_TO_CHANNEL7_OFFSET 52
#define IMPORT_TO_CHANNEL8_OFFSET 53
#define IMPORT_TO_CHANNEL9_OFFSET 54
#define IMPORT_TO_CHANNEL10_OFFSET 55
#define IMPORT_TO_CHANNEL11_OFFSET 56
#define IMPORT_TO_CHANNEL12_OFFSET 57
#define IMPORT_TO_CHANNEL13_OFFSET 58
#define IMPORT_TO_CHANNEL14_OFFSET 59
#define IMPORT_TO_CHANNEL15_OFFSET 60*/
//#define IMPORT_TO_ 45

//#define IMPORT_TO_DELTAX 19

struct ColumnFormat
{
int format;
int size;
};

struct BinReadInstructions
{
unsigned int length_of_header;//number of bytes at the beginning of the file before reading starts
unsigned int column_count;//number of columns that are saved in the binary file
struct ColumnFormat * column_type;//tells us the number of bytes for the representation of a specific column and its format
unsigned int datasets_to_read;//max set-count to be read
bool read_until_end_is_reached;//read as much as possible
unsigned int n_columns_to_read;//number of columns to be saved
int * col_numbers_to_read;//which columns are to be read and where they are to be stored
};

//void store_plot_string(plotstr * p,stdLineEdit * led);

void LoadBinaryData(int gno,int setno,int settype,char * filename,struct BinReadInstructions inst);

struct FoundSetID
{
int pos;
int len;//the length includes control characters for the characteristic
int gno,sno;
int characteristic;//0=basic set id; 1=constant set-id; 2=set-id to be increased by offset
int color;
int repl_gno,repl_sno;
};

void find_set_ids(char * str,int * nr,struct FoundSetID ** foundIds);//tries to find complete set-ids like "G0.S2" inside a string and returns the number of found ids (nr), there positions inside the string (startpos), their length (len) and the graph- and set-numbers (gnos/snos)
void find_graph_ids(char * str,int * nr,struct FoundSetID ** foundIds);//tries to find sigular appearances of Graph-ids without set ids
void postprocess_found_set_ids(int nr,struct FoundSetID ** foundIds,int * nr_unique_ids,int ** unique_id);//tries to identifiy multiple occurences of the same set and gives every set-id a color matching the color in the graph - if possible; unique_id gives the position of a singular id in the set-ids
QString CreateRichTextColorTable(void);
QString PaintSetIds(char * str,int nr,struct FoundSetID * setIds);//Creates a html-representation for str with colored set-ids
QString ReplaceSetIds(char * str,int nr,struct FoundSetID * setIds);
QString ReplaceGraphIds(char * str,int nr,struct FoundSetID * setIds);
QString ColorToHtml(int nr);
QString ColorHtmlEnd(void);
QIcon createSingleSmallColorIcon(int col);

extern int GetSpinChoice(stdIntSelector * s);
extern double GetSpinChoice(LineWidthSelector * s);
extern double GetSpinChoice(QDoubleSpinBox * s);
extern bool GetToggleButtonState(QCheckBox * c);
extern bool GetToggleButtonState(QAction * c);
extern int GetChoice(StdSelector * s);
extern int GetChoice(PositionSelector * s);
extern int GetOptionChoice(ColorSelector * s);
extern int GetOptionChoice(LineStyleSelector * s);
extern int GetOptionChoice(FillPatternSelector * s);
extern int GetOptionChoice(JustificationSelector * s);
extern int GetOptionChoice(FontSelector * s);
extern int GetOptionChoice(QComboBox * c);
extern int GetOptionChoice(StdSelector * sel);
extern double GetCharSizeChoice(stdSlider * sel);
extern double GetAngleChoice(stdSlider * sel);
extern char * GetTextString(stdLineEdit * led);
extern void SetToggleButtonState(QCheckBox * cb, int v);
extern void SetToggleButtonState(QAction * c, int v);
extern void SetChoice(StdSelector * sel,int v);
extern void SetChoice(PositionSelector * sel,int v);
extern void SetOptionChoice(ColorSelector * sel,int v);
extern void SetOptionChoice(LineStyleSelector * sel,int v);
extern void SetOptionChoice(FillPatternSelector * sel,int v);
extern void SetOptionChoice(FontSelector * sel,int v);
extern void SetOptionChoice(JustificationSelector * sel,int v);
extern void SetOptionChoice(StdSelector * sel,int v);
extern void SetSpinChoice(stdIntSelector * sel, double v);
extern void SetSpinChoice(LineWidthSelector * sel, double v);
extern void SetScaleValue(stdSlider * sel, int v);
extern void SetCharSizeChoice(stdSlider * sel,double v);
extern void SetAngleChoice(stdSlider * sel,double v);
extern void SetTextString(stdLineEdit * led,const char *st);
extern void SetLabel(QLabel * lab,const char *st);
extern void xv_setstr(stdLineEdit * led, const char *text);
extern void xv_setstr(QLineEdit * led, const char *text);
extern char * xv_getstr(stdLineEdit * led);
extern char * xv_getstr(QLineEdit * led);
extern void SetSensitive(stdLineEdit * led, bool e);
extern void SetSensitive(QLineEdit * led, bool e);
extern void SetSensitive(StdSelector * led, bool e);
extern void SetSensitive(QPushButton * led, bool e);
extern void SetSensitive(QCheckBox * led, bool e);
extern int GetScaleValue(stdSlider * sel);
extern int xv_evalexpr(stdLineEdit * led,double * val);
extern int std_evalexpr(char * buf,double * val);
extern int xv_evalexpri(stdLineEdit * led,int * val);
extern int xv_evalexpr(QLineEdit * led,double * val);
extern int xv_evalexpri(QLineEdit * led,int * val);
extern void SelectListChoice(uniList * list,int nr);
extern void SelectListChoices(uniList * list,int count,int * nrs);
extern int GetListChoices(uniList * list, int ** sel);
extern int GetSelectedSets(uniList * list, int ** sel);
extern int GetSelectedSet(uniList * list);
extern int GetSingleListChoice(uniList * list, int * sel);

#if QT_VERSION < 0x050000
        #include <QPlastiqueStyle>
        #include <QCleanlooksStyle>
        #include <QWindowsStyle>
    #if QT_VERSION < 0x040500
        #define MYSTYLE QWindowsStyle
    #else
        #define MYSTYLE QPlastiqueStyle
    #endif

#else
    #include <QCommonStyle>
    #include <QStyle>
    #define MYSTYLE QCommonStyle
#endif

class skfStyle:public MYSTYLE
{
public:
    skfStyle(int marginIn,int spacingIn):MYSTYLE(),margin(marginIn),spacing(spacingIn) {};
    virtual ~skfStyle(){};
    int margin;
    int spacing;
    virtual int pixelMetric ( PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const {
        // BTRACEF(Int2Str(QStyle::PM_SmallIconSize)); // STRANGE IN 67.73
        int retval=MYSTYLE::pixelMetric(metric,option,widget);
        if (retval!=-1){
            //   if ((margin<0)||(spacing<0)) return MYSTYLE::pixelMetric(metric,option,widget);
            if (metric==QStyle::PM_LayoutTopMargin) retval=margin;
            if (metric==QStyle::PM_LayoutRightMargin) retval=margin;
            if (metric==QStyle::PM_LayoutBottomMargin) retval= margin;
            if (metric==QStyle::PM_LayoutLeftMargin) retval= margin;
            if (metric==QStyle::PM_LayoutHorizontalSpacing) retval= spacing;
            if (metric==QStyle::PM_LayoutVerticalSpacing) retval= spacing;
            if (metric==QStyle::PM_ButtonMargin) retval= spacing;
            if (metric==QStyle::PM_ToolBarIconSize) retval=32;
            if (metric==QStyle::PM_MenuBarItemSpacing) retval=20;
            // if (metric==77) retval= spacing;
        }
         return retval;
    };
};

class formula_to_process//a class to preprocess formulas for the Grace-parser
{
public:
formula_to_process(void);
~formula_to_process();
int len_orig_formula;
QString original_formula;
int current_graph,current_set;//in formula replacement (if incomplete set ids are used)
//when a formula is initialized, it is analysed for tokens - for easier replacement
QList<int> token_startpos;//absolute position in the original string
QList<int> token_type;//the type of the id
QList<int> token_len;//the length of the id (for replacing later)
QList<int> token_id1;
QList<int> token_id2;//the actual id in the original formula (id1=graph-id; id2=set-id)
QList<int> token_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
QList<char> token_control1;//control-character for Graph-ID ('$','#','N')
QList<char> token_control2;//control-character for Set-ID
QList<int> token_column;//[ X=0 , Y=1 , Y1=2 , Y2=3 , Y3=4 ]
//please note: all lists have the same length: the number of found tokens
//the following lists are the allocated replacement-ids for new- or counter-ids
QList<int> token_graph_new;
QList<int> token_set_new;
QList<int> token_graph_counter;
QList<int> token_set_counter;
QList<bool> token_graph_counter_used;
QList<bool> token_set_counter_used;

void process_single_string(QString text,QList<int> & t_startpos,QList<int> & t_type,QList<int> & t_len,QList<int> & t_id1,QList<int> & t_id2,QList<char> & t_control1,QList<char> & t_control2,QList<int> & t_colum);//this is for a single formula-section (without `"`or `=`)
static bool has_fit_formula_command(QString f);
static int is_comment(QString f);
void sync_allocated_new_and_counter(class formula_to_process * other_formula);//takes the replacement settings from another formula --> needed not to reallocate new graphs/sets if working with more than one formula-item
void init_formula(QString f);//clear everything and enter new formula
void add_one_token(int startpos,int type,int len,int id1,int id2,int pos,char control1,char control2,int col);
void return_qtspecial_subcommand(int nr,int type,int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & f);
void return_completed_formula(int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & f);//returns a formula with completed ids (complete ids (like G0.S1.Y) stay untouched, incomplete ids (like just Y) get completed according to their position on the left hand side (lhs) or right hand side (rhs) of a '=')
void return_replaced_formula(int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & f);//completes incomplete ids and replaces existing ids
void check_destination_set_existence(int g_lhs,int s_lhs,int g_rhs,int s_rhs,int replace);//generates destination sets if needed
QString execute_formula(int g_lhs,int s_lhs,int g_rhs,int s_rhs,int replace,int & result);//if replace=1 --> replace ids, otherwise: just complete ids (no replacing)
void clear_replacements_and_counters(void);
void increase_counters(void);
int get_graph_counter(int id1);
int get_set_counter(int id1, char control1, int id2);
int get_new_graph(int id1);
int get_new_set(int id1, char control1, int id2);
int get_current_graph_id(int nr);
int get_current_set_id(int nr);
void get_List_of_Subformulas(QString text,QStringList & subtext,QList<int> & offsets);
void clear_tokens(void);
void return_single_token(int nr, int type, int g_lhs,int s_lhs,int g_rhs,int s_rhs, QString & new_token);//returns one token (nr in the list) in its new form (type=0 --> original | type=1 --> completed if incomplete | type=2 --> replaced)
void get_string_after_token(int nr,QString & text);//get formula content between the token (nr) and the next token; if nr=-1 --> beginning before first token
};

#endif
