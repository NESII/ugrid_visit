/*****************************************************************************
*
* Copyright (c) 2000 - 2015, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                            avtUGRIDFileFormat.h                           //
// ************************************************************************* //

#ifndef AVT_UGRID_FILE_FORMAT_H
#define AVT_UGRID_FILE_FORMAT_H

#include <avtMTSDFileFormat.h>
#include <vtkUnstructuredGrid.h>

#include <vector>
#include <map>

#define MAX_SIDES 50

class VarInfo {
public:
  std::string name;
  std::string mesh_name;

  // these are -1 if not present, and otherwise
  // give where the respective dimension falls in the variable definition.
  int time_dimi,cell_dimi,layer_dimi,node_dimi;
  // really these ought to be stored in a mesh object - shared among variables
  // which live on the same grid.
  int time_dim,cell_dim,layer_dim,node_dim;
  int var_id,ncid;

  VarInfo(std::string);
  VarInfo(void);
  VarInfo(const VarInfo &);
  void init(void);
  
  float *read_cell_at_time(int);
  float *read_cell_z_at_time(int);
};

// ****************************************************************************
//  Class: avtUGRIDFileFormat
//
//  Purpose:
//      Reads in UGRID files as a plugin to VisIt.
//
//  Programmer: rusty -- generated by xml2avt
//  Creation:   Sat Mar 5 18:27:45 PST 2016
//
// ****************************************************************************

class avtUGRIDFileFormat : public avtMTSDFileFormat
{
  public:
  avtUGRIDFileFormat(const char *);

  virtual ~avtUGRIDFileFormat();

    //
    // This is used to return unconvention data -- ranging from material
    // information to information about block connectivity.
    //
    // virtual void      *GetAuxiliaryData(const char *var, int timestep, 
    //                                     const char *type, void *args, 
    //                                     DestructorFunction &);
    //

    //
    // If you know the times and cycle numbers, overload this function.
    // Otherwise, VisIt will make up some reasonable ones for you.
    //
    // virtual void        GetCycles(std::vector<int> &);
    // virtual void        GetTimes(std::vector<double> &);
    //

    virtual int            GetNTimesteps(void);
    virtual void           GetTimes(std::vector<double> &);
    virtual void           GetCycles(std::vector<int> &);

    virtual const char    *GetType(void)   { return "UGRID"; };
    virtual void           FreeUpResources(void); 

    virtual vtkDataSet    *GetMesh(int, const char *);
    virtual vtkDataArray  *GetVar(int, const char *);
    virtual vtkDataArray  *GetVectorVar(int, const char *);

  vtkUnstructuredGrid *GetMeshNodes(const std::string);
  vtkDataSet *ExtrudeTo3D(const std::string,int,vtkUnstructuredGrid *);
  vtkPoints *GetNodes(const std::string);

protected:
  // DATA MEMBERS

  virtual void           PopulateDatabaseMetaData(avtDatabaseMetaData *, int);

  int ncid; // handle for netcdf file
  int time_dim,node_dim,cell_dim,layer_dim;
  int time_var, mesh_var;
  int node_x_var,node_y_var;
  std::string default_ugrid_mesh;
  std::map<std::string,VarInfo> var_table;


  // basic dimensions 
  size_t n_nodes,n_cells,n_layers;

  // map 3D cell ids to real cells, because cells that are
  // underground are not output.
  std::map<int,int> full_cell2valid;


  // netcdf helpers:
  // read a full field of values at a given timestate
  // layer is the fastest changing index
  float *read_cell_z_full(std::string,int);

  std::string var_mesh(std::string);
  std::string var_mesh(int);

  void activateTimestate(int);
  // various geometry-related data specific to the active timestate
  int active_timestate;
  int *cell_kmin;
  int *cell_kmax;
  int ncells_3d;

  vtkDataArray *GetVar3D(int, VarInfo &);
  vtkDataArray *GetVar2D(int, VarInfo &);
};


#endif
