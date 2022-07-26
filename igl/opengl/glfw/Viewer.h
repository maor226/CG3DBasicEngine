// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2014 Daniele Panozzo <daniele.panozzo@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#ifndef IGL_OPENGL_GLFW_VIEWER_H
#define IGL_OPENGL_GLFW_VIEWER_H

#ifndef IGL_OPENGL_4
#define IGL_OPENGL_4
#endif

#include "../../igl_inline.h"
#include "../MeshGL.h"

#include "../ViewerData.h"
#include "ViewerPlugin.h"
#include "igl/opengl/Movable.h"
#include "igl/opengl/glfw/Material.h"


#include <Eigen/Core>
#include <Eigen/Geometry>

#include <vector>
#include <list>
#include <string>
#include <cstdint>

#define IGL_MOD_SHIFT           0x0001
#define IGL_MOD_CONTROL         0x0002
#define IGL_MOD_ALT             0x0004
#define IGL_MOD_SUPER           0x0008

#define POINTS_NUM 7
#define cur_layer (layer_index + 1)
#define single_picked (single_picked_shape_idx != -1)
#define picking_shader 0

#define SCREEN_WIDTH 100.0
#define SCREEN_HEIGHT SCREEN_WIDTH

using namespace std;

class Shape 
{
private:
	Eigen::Vector2d* get_points(int section){
		return bez_points + section*3;
	}
  double t, dt;
  int section;

public:
  Eigen::Vector3d animate_pos, edit_pos;
	Eigen::Vector2d bez_points[POINTS_NUM];
  int shapeIdx;
  int layer;
  bool *picked;

	Shape(int _shapeIdx, int _layer) {
		double x = 0.5528;
		bez_points[0] = Eigen::Vector2d(-4, 0);
		bez_points[1] = Eigen::Vector2d(-1, x);
		bez_points[2] = Eigen::Vector2d(-x, 1);
		bez_points[3] = Eigen::Vector2d(0, 4);
		bez_points[4] = Eigen::Vector2d(x, 1);
		bez_points[5] = Eigen::Vector2d(1, -x);
		bez_points[6] = Eigen::Vector2d(-4, 0);

    shapeIdx = _shapeIdx;
    edit_pos = animate_pos = Eigen::Vector3d(0, 0, 0);
    t = 0;
    dt = 0.01; 
    section = 0;
    layer = _layer;
    picked = new bool(true);
	}

	Eigen::Vector2d bezier(double t, int section){
		Eigen::Vector2d* points = get_points(section);
		double t_1 = t, t_2 = t*t, t_3 = t_1 * t_2;
		double tc_1 = (1 -t), tc_2 = (1 -t)*(1 - t), tc_3 = tc_1 * tc_2;

		return tc_3 * points[0] + 3 *tc_2 * t_1 * points[1] + 3* tc_1 * t_2 * points[2] + t_3 * points[3];
	}

	Eigen::Vector3d velocity(){
		Eigen::Vector2d bez_t = bezier(t, section);
		Eigen::Vector2d bez_p = bezier(t + dt, section);
		Eigen::Vector3d temp(bez_p[0] - bez_t[0], bez_p[1] - bez_t[1] ,0);
		//std::cout<<_t<< "\n"<<dt<< "\n" << temp << "\n\n";
		return temp; 
	}

  Eigen::Vector3d step_animate() {
		//std::cout << t<< std::endl;
		int maxSegmentNum = 1;//=((Bezier1D*)data_list[currIndx])->GetSegmentsNum();
		if(t<=1 && t >=0){ 
			Eigen::Vector3d vel = velocity();
			if (dt <0)
				vel = Eigen::Vector3d(vel[0],-1*vel[1],0);
			// std::cout << "Animate" <<GetVelocity(segment, t, dt) <<std::endl << t <<std::endl<< dt <<std::endl<< segment<< std::endl;
		  animate_pos += vel;
			//data_list[3]->MyTranslate(((Bezier1D*)data_list[curIndx])->GetVelocity(segment,1-t,dt),1); //todo GetVelocity === שיפוע
		  std::cout << this->t << "\n";
      t+=dt;
      if(t > 1 - std::abs(dt)  && t < 1)
        t = 1;
      if(t < 0 + std::abs(dt) && t > 0)
        t=0;
      std::cout << t << "\n";
      return vel;
    } else if(t>=1){
			if(section == maxSegmentNum){
				dt = -dt;
				t =1;
			}else{
				section++;
				t = dt;
			}
		}else if(t<=0){
			if(section == 0){
				t = 0;
				dt = -dt;
        //return animate_pos - edit_pos; //floating point error
			}else{
				section--;
				t = 1;
			}
		}
    return Eigen::Vector3d(0, 0, 0);
  }

  void reset_animation() {
    t = 0;
		dt = abs(dt);
		section = 0;
		animate_pos = edit_pos;
  }

};

namespace igl
{
namespace opengl
{
namespace glfw
{
  // GLFW-based mesh viewer
  class Viewer : public Movable
  {
  protected:
    bool change_bez = true;
  public:
      bool pick = false;
      int single_picked_shape_idx = -1; 
      int shape_index = 0;
      std::vector<Shape> shapes;

      //for gui menu
      int layer_index = 0;
      int material_idx = 0;
      std::vector<bool*> picked_shapes;
      std::vector<bool*> show_layer;
      vector<string> material_names;
      vector<string> shape_names;


      enum axis { xAxis, yAxis, zAxis };
      enum transformations { xTranslate, yTranslate, zTranslate, xRotate, yRotate, zRotate, xScale, yScale, zScale,scaleAll,reset };
      enum modes { POINTS, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, QUADS };
      enum shapes {Axis, xCylinder,yCylinder,zCylinder, Plane, Cube, Octahedron, Tethrahedron, LineCopy, MeshCopy, Sphere, Bezier_Line };
      enum buffers { COLOR, DEPTH, STENCIL, BACK, FRONT, NONE };
    // UI Enumerations
   // enum class MouseButton {Left, Middle, Right};
   // enum class MouseMode { None, Rotation, Zoom, Pan, Translation} mouse_mode;
    virtual void Init(const std::string config);
	virtual void Animate() {}
	virtual void WhenTranslate() {}
	virtual Eigen::Vector3d GetCameraPosition() { return Eigen::Vector3d(0, 0, 0); }
	virtual Eigen::Vector3d GetCameraForward() { return Eigen::Vector3d(0, 0, -1); }
	virtual Eigen::Vector3d GetCameraUp() { return Eigen::Vector3d(0, 1, 0); }

	//IGL_INLINE void init_plugins();
    //IGL_INLINE void shutdown_plugins();
    Viewer();
    virtual ~Viewer();
      virtual void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx){};
      virtual void Update_overlay(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int shapeIndx,bool is_points);
      virtual int AddShape(int type, int parent, unsigned int mode, int viewport = 0);
      virtual int AddShapeFromFile1(const std::string& fileName, int parent = -1, unsigned int mode = TRIANGLES, int viewport = 0);
      virtual int AddShapeFromFile(const std::string& fileName, int parent, unsigned int mode, int viewport = 0);
      virtual void WhenTranslate(float dx, float dy) {}
      virtual void WhenRotate(float dx, float dy) {}
      virtual void WhenScroll(float dy) {}

      string get_name_from_path(const std::string& path);
      void ChangePickedShapeMaterial();
      void open_dialog_load_texture();

      //check if single picked and if so update picked_shape_idx
      void changePickedShape() {
        //should update bez curves
        change_bez = true;

        int picked = -1;
        for(int i = 0; i <shapes.size(); i++){
          if(*(shapes[i].picked)) {
            if(picked == -1)
              picked = i;
            else {
              single_picked_shape_idx = -1;
              return;
            }
          }
        }
        pick = (picked != -1);
        single_picked_shape_idx = picked;
      }
    // Mesh IO
    IGL_INLINE bool load_mesh_from_file(const std::string & mesh_file_name);
    IGL_INLINE bool save_mesh_to_file(const std::string & mesh_file_name);
   
    // Scene IO
    IGL_INLINE bool load_scene();
    IGL_INLINE bool load_scene(std::string fname);
    IGL_INLINE bool save_scene();
    IGL_INLINE bool save_scene(std::string fname);
    // Draw everything
   // IGL_INLINE void draw();
    // OpenGL context resize
   
    // Helper functions

    IGL_INLINE void open_dialog_load_mesh();
    IGL_INLINE void open_dialog_save_mesh();

	IGL_INLINE void draw() {}
    ////////////////////////
    // Multi-mesh methods //
    ////////////////////////

    // Return the current mesh, or the mesh corresponding to a given unique identifier
    //
    // Inputs:
    //   mesh_id  unique identifier associated to the desired mesh (current mesh if -1)
    IGL_INLINE ViewerData* data(int mesh_id = -1);
    IGL_INLINE const ViewerData* data(int mesh_id = -1) const;

    // Append a new "slot" for a mesh (i.e., create empty entries at the end of
    // the data_list and opengl_state_list.
    //
    // Inputs:
    //   visible  If true, the new mesh is set to be visible on all existing viewports
    // Returns the id of the last appended mesh
    //
    // Side Effects:
    //   selected_data_index is set this newly created, last entry (i.e.,
    //   #meshes-1)
    IGL_INLINE int append_mesh(bool visible = true);

    // Erase a mesh (i.e., its corresponding data and state entires in data_list
    // and opengl_state_list)
    //
    // Inputs:
    //   index  index of mesh to erase
    // Returns whether erasure was successful <=> cannot erase last mesh
    //
    // Side Effects:
    //   If selected_data_index is greater than or equal to index then it is
    //   decremented
    // Example:
    //   // Erase all mesh slots except first and clear remaining mesh
    //   viewer.selected_data_index = viewer.data_list.size()-1;
    //   while(viewer.erase_mesh(viewer.selected_data_index)){};
    //   viewer.data().clear();
    //
    IGL_INLINE bool erase_mesh(const size_t index);

    // Retrieve mesh index from its unique identifier
    // Returns 0 if not found
    IGL_INLINE size_t mesh_index(const int id) const;

	Eigen::Matrix4d CalcParentsTrans(int indx);
	inline bool SetAnimation() { return isActive = !isActive; }
    inline  bool  IsActive() const { return isActive; }
    inline void Activate() { isActive = true; }
    inline void Deactivate() { isActive = false; }
    int AddShader(const std::string& fileName);
public:
    //////////////////////
    // Member variables //
    //////////////////////

    // Alec: I call this data_list instead of just data to avoid confusion with
    // old "data" variable.
    // Stores all the data that should be visualized
    std::vector<ViewerData*> data_list;
    std::vector<int> pShapes;
	std::vector<int> parents;
    std::vector<Texture*> textures;
    std::vector<Material*> materials;
    std::list<int> pickedShapes;
    Eigen::Vector3d pickedNormal;
    int selected_data_index;
    int next_data_id;
    int next_shader_id; // for flags to mack sure all shaders are initlize with data
	bool isActive;
    unsigned int staticScene;

    Shader* overlay_shader;
    Shader* overlay_point_shader;
    std::vector<Shader*> shaders;

    

    // List of registered plugins
//    std::vector<ViewerPlugin*> plugins;

    // Keep track of the global position of the scrollwheel
    float scroll_position;

  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

      void
      Draw(int shaderIndx, const Eigen::Matrix4f &Proj, const Eigen::Matrix4f &View, int viewportIndx,
           unsigned int flgs,unsigned int property_id);



      void ClearPickedShapes(int viewportIndx);

      int AddMaterial(unsigned int *texIndices, unsigned int *slots, unsigned int size);

      Eigen::Matrix4d GetPriviousTrans(const Eigen::Matrix4d &View, unsigned int index);

      float
      AddPickedShapes(const Eigen::Matrix4d &PV, const Eigen::Vector4i &viewport, int viewportIndx, int left, int right,
                      int up, int bottom, int newViewportIndx);

      void
      MouseProccessing(int button, int xrel, int yrel, float movCoeff, Eigen::Matrix4d cameraMat, int viewportIndx);

      virtual void WhenTranslate(const Eigen::Matrix4d &preMat, float dx, float dy);

      virtual void WhenScroll(const Eigen::Matrix4d &preMat, float dy);

      virtual void WhenRotate(const Eigen::Matrix4d &preMat, float dx, float dy);

      int AddTexture(const std::string& textureFileName, int dim);
      int AddTexture(int width, int height, unsigned char* data, int mode);
      void BindMaterial(Shader* s, unsigned int materialIndx);
      void BindTexture(int texIndx, int slot) { textures[texIndx]->Bind(slot); }
      IGL_INLINE void SetShapeShader(int shpIndx, int shdrIndx) { data_list[shpIndx]->SetShader(shdrIndx); }
      IGL_INLINE void SetShapeStatic(int shpIndx) { data_list[shpIndx]->SetStatic(); }
      IGL_INLINE void SetShapeViewport(int shpIndx, int vpIndx) { vpIndx>0 ? data_list[shpIndx]->AddViewport(vpIndx) : data_list[shpIndx]->RemoveViewport(~vpIndx); }
      inline void UpdateNormal(unsigned char data[]) { pickedNormal = (Eigen::Vector3d(data[0], data[1], data[2])).normalized(); }
      IGL_INLINE void SetShapeMaterial(int shpIndx, int materialIndx) { data_list[shpIndx]->SetMaterial(materialIndx); }

      void SetShader_overlay(const std::string &fileName);

      void SetShader_point_overlay(const std::string &fileName);

      int AddShapeCopy(int shpIndx, int parent, unsigned int mode, int viewport = 0);

      void ShapeTransformation(int type, float amt, int mode);

      virtual bool Picking(unsigned char data[4], int newViewportIndx);
      inline void UnPick() { selected_data_index = -1; pickedShapes.clear(); }

      bool load_mesh_from_data(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F, const Eigen::MatrixXd &UV_V,
                               const Eigen::MatrixXi &UV_F);

      int AddShapeFromData(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F, const Eigen::MatrixXd &UV_V,
                           const Eigen::MatrixXi &UV_F, int type, int parent, unsigned int mode, int viewport);

      int AddShader(const std::string &Vertex_Shader, const std::string &Fragment_shader);

      void SetParent(int indx, int newValue, bool savePosition);
  };

} // end namespace
} // end namespace
} // end namespace

#ifndef IGL_STATIC_LIBRARY
#  include "Viewer.cpp"
#endif

#endif
