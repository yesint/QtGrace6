/***************************************************************************
 *   Copyright (C) 2008-2022 by Andreas Winter                             *
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

#ifndef UNDO_MODULE_H
#define UNDO_MODULE_H

#include <QtGui>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include "noxprotos.h"
#include "t1fonts.h"
#include "parser.h"
#include "graphs.h"
#include "utils.h"
#include "files.h"
#include "globals.h"
#include "defines.h"
#include "draw.h"

#define MAX_UNDO_NODES 100

//min 4
#define MAX_DATA_IN_NODE 4

#define UNDO_TYPE_INVALID 0
//remember: multiple is possible!
#define UNDO_TYPE_KILL_SET 1
#define UNDO_TYPE_NEW_SET -1
#define UNDO_TYPE_OBJECT_DELETED 2
#define UNDO_TYPE_OBJECT_INSERTED -2
#define UNDO_TYPE_KILL_GRAPH 3
#define UNDO_TYPE_NEW_GRAPH -3

//data changed, appearance unchanged
#define UNDO_TYPE_SET_MODIFIED 100
//data unchanged, appearance changed
#define UNDO_TYPE_SET_APPEARANCE 101
//appearance of graph(s) changed
#define UNDO_TYPE_GRAPH_APPEARANCE 102
//plot appearance changed
#define UNDO_TYPE_PLOT_APPEARANCE 103
//page setup changed
#define UNDO_TYPE_PAGE_SETUP 104
//axis properties modified
#define UNDO_TYPE_AXIS_PROPERTIES 105

#define UNDO_TYPE_OBJECT_MODIFIED 106

//set or graph is selected in origin[1] (0=set,1=graph)
#define UNDO_TYPE_NEW_ORDERING 107
#define UNDO_TYPE_SET_SWAP 108
#define UNDO_TYPE_GRAPH_SWAP 109
#define UNDO_TYPE_PACK_SETS 110

#define UNDO_TYPE_FIT 111
#define UNDO_TYPE_REGION 112
#define UNDO_TYPE_VIEWPORT_CHANGED 113
#define UNDO_TYPE_DATAPOINT_EDIT 114
#define UNDO_TYPE_SHOW_HIDE 115
#define UNDO_TYPE_DEVICE_MODIFIED 116
#define UNDO_TYPE_HOTLINK 117
#define UNDO_TYPE_LOCATOR_FIXPOINT 118
#define UNDO_TYPE_IMPORT_ASCII 119
#define UNDO_TYPE_IMPORT_BLOCK_DATA 120
#define UNDO_TYPE_COLOR_MAP_CHANGED 121
#define UNDO_TYPE_SET_REGRESSION 122
#define UNDO_TYPE_SET_FILTER 123
#define UNDO_TYPE_IMPORT_SET_FROM_AGR 124
#define UNDO_TYPE_CHANGE_SET_ORDER 125
//#define UNDO_TYPE_OBJECT_MOVED 14 //moved = modified
//#define UNDO_GENERIC 14

//the following markers are to specify copy- and reinstall-operations
//complete: everything, including axes, data, settings and labels
//appearance: only settings (viewport, coordinates and so on) and labels
//data: only data in sets and sets in graphs
//axis: only the tickmarks on different axes
#define UNDO_COMPLETE 0
#define UNDO_APPEARANCE 1
#define UNDO_DATA 2
#define UNDO_AXIS 4

#define UNDO_SET 0
#define UNDO_GRAPH 1

using namespace std;

struct all_fit_settings
{
int src_gno,dest_gno;
int nr_sr_sets;
int * src_sets,dest_sets;
int nr_of_parameters;
int iterations;
int restrictions;
int weights,load;
QString formula;
QString Tolerance;
QString weight_func;
QString start,stop,nr;
QString value[MAXPARM+1],lower[MAXPARM+1],upper[MAXPARM+1];
bool bound[MAXPARM+1];
bool negated;
};

class undo_node
{
public:
undo_node(void);
~undo_node();

bool active;
bool multiple;
int type;//what sort of action is to be made undone
bool saved;//does there exist a saved status to speed up undo-process
int this_node_nr;
QString savedFileName;
QString Description;
QString AdditionalDescription;
QStringList Changes,OldStates;
//general purpose data registers
int origin[MAX_DATA_IN_NODE];
int * id[MAX_DATA_IN_NODE];
double * ddata[MAX_DATA_IN_NODE];
void * data;
plotarr ** p;

void reStoreData(void);
void clearContents(void);
QString getDescription(void);
int containsSet(int gno,int setno);
};

void initNodes(void);
undo_node * getNextNode(void);//to get the next valid Node to write data into
undo_node * getPreviousNode(void);
undo_node * getNodeByNr(int i);
void NextNode(void);//to increase the Node-Count (used to finish writing to a node)
void PreviousNode(void);
int getNrOfNodes(void);
QString getNodeDescription(int i);
void RestoreNode(int i);
int get_inverse_action(int i);
int index_translator(int i);
int nextUndoNode(void);//returns the real index of the next Node to be made undone
int nextRedoNode(void);//returns the real index of the next Node to be redone

void Undo(void);
void Redo(void);
bool UndoPossible(void);
bool RedoPossible(void);

void CheckActive(void);

void ObjectsDeleted(int len,int * ids,int type);
void ObjectsCreated(int len,int * ids,int type);

void SetsDeleted(int len,int * gnos,int * snos,int what);
void SetsCreated(int len,int * gnos,int * snos,int what);
void SetImported(int gno,int setno,char * filename,int cursource,int load,int autoscale);
void SetsImported(int len,int * gnos,int * snos,int files,char ** filename,int cursource,int load,int autoscale);
void SetsImportedFromAgr(int len,int * snos,struct agr_file_info afi,int autoscale);
void SetImportBlockData(int gno, int setno,char * filename,int source, int nc, int *coli, int scol,int load,int autoscale);
void SetNewSetNo(int setno);
//void SetImportBinaryData(int gno,int setno,char * filename,struct importSettings * set);
void SetsReorderd(int gno,QList<int> new_set_order);

void SetRegression(int n_sets,int * gnos,int * snos,int n_n_sets,int * n_gnos, int * n_snos, int ideg, int iresid, int rno, int invr,double start,double stop,int points,int rx,char * formula);
void SetFilter(int o_n_sets,int * o_gnos,int * s_nos,int n_n_sets,int * n_gnos, int * n_setnos,int type,int realization,double * limits,int * orders,char * x_formula,double ripple,int absolute,int debug,int point_extension,int oversampling,int rno,int invr);

void GraphsDeleted(int len,int * gnos,int what);
void GraphsCreated(int len,int * gnos,int what);

void SaveSingleSetStatePrevious(int gno,int setno,int what);
void SaveSetStatesPrevious(int len,int * gnos,int * snos,int what);//call this before the modification
void SingleSetModified(int gno,int setno,int what);
void SetsModified(int len,int * gnos,int * snos,int what);//call this after the modification

void SaveSingleGraphState(int gno,int what);
void SaveGraphStatesPrevious(int len,int * gnos,int what);//call this before the modification
void SingleGraphModified(int gno,int what);
void GraphsModified(int len,int * gnos,int what);//call this after the modification

void SaveSingleGraphTickmarks(int gno);
void SaveTickmarksStatesPrevious(int axis_start,int axis_stop,int nr_of_graphs,int * graph_nrs);
void SaveTickmarksStatesPrevious(int axis_start,int axis_stop,int graph_start,int graph_stop);
void SaveTickmarksStatesPrevious(int nr_of_axes,int * axes,int nr_of_graphs,int * graph_nrs);
void SingleGraphTickmarksModified(int gno);
void TickmarksModified(int axis_start,int axis_stop,int nr_of_graphs,int * graph_nrs);
void TickmarksModified(int axis_start,int axis_stop,int graph_start,int graph_stop);
void TickmarksModified(int nr_of_axes,int * axes,int nr_of_graphs,int * graph_nrs);

void SaveDeviceState(int id,bool sync);
void DeviceModified(int id,bool sync);

void SavePlotAppearance(void);
void PlotAppearanceModified(void);

void SaveObjectData(int id,int type);
void ObjectDataModified(int id,int type);

void SaveHotlinkData(int len,int * gnos,int * snos);
void HotlinkModified(int len,int * gnos,int * snos);

void SaveLocatorFixPoint(int gno);
void LocatorFixPointModified(int gno);

void copySet(int gno,int setno,plotarr * destination,int what);
void deleteSavedSet(plotarr * destination,int what);
void reinstallSet(int gno,int setno,plotarr * destination,int what);

void copyGraph(int gno,graph * destination,int what);
void deleteSavedGraph(graph * destination,int what);
void reinstallGraph(int gno,graph * destination,int what);

void updateUndoList(void);
void addAditionalDescriptionToLastNode(int type,QString description,QString additional,int mult=-1);//usefull for adding a formula or changing the description; setting something as -1 or empty means: let this entry be unchanged

void UndoSwapGraphs(int g1,int g2);
void UndoSwapSets(int g1,int s1,int g2,int s2);
void SetsAboutToBePacked(int gno);

void ViewportChanged(int gno,view v);
void copy_Datapoint(int gno,int sno,int nr,Datapoint * p);
void DataPointEdited(int gno,int sno,int * nrs,int len,Datapoint * p,int type);//type=0: new point, type=1: point deleted, type=2: point modified

void CopyRegion(region * from,region * to);
void DeleteRegion(region * regi);
void RegionModified(int nr,region * newregion,int type);//type=0 modified, type=1 killed

int contains(int * list,int len,int number);//returns the (first) position of number in list or -1 if number not found
int ContainsGS(int * gnos,int * snos,int len,int gno,int setno);

void SaveFitSettings(void);
void SaveFitAfter(void);
void get_fit_settings(struct all_fit_settings * fit);
void set_fit_settings(struct all_fit_settings * fit);

void SaveColMapToMemory(int * nr,CMap_entry ** table);
void DeleteColMapFromMemory(int * nr,CMap_entry ** table);
void IntstallColorMap(int nr, CMap_entry* colors);
void SaveCurrentColorMap(void);
void CurrentColorMapChanged(void);

void ShowHideSets(int len,int * gnos,int * sets);
void ShowHideGraphs(int len,int * gnos);

void ChangeSetOrdering(int gno,int setno,int type);//type=PUSH_SET_TOFRONT / PUSH_SET_TOBACK

int NodeContainsKilledSet(int startnode,bool forward,int gno,int setno);//looks into other nodes to find out whether a node contains the specified set

/*void CopyBinaryImportSettings(struct importSettings * from,struct importSettings * to);
void DeleteBinaryImportSettings(struct importSettings * set);*/
void getHotlinkedSets(int * nr,int ** gnos,int ** snos);

#endif // UNDO_MODULE_H
