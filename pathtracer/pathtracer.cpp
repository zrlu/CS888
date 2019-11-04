// ======================================================================== //
// Copyright 2009-2018 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "../common/tutorial/tutorial.h"

namespace embree
{
  extern "C" {
    int g_spp = 4;
    int g_max_path_length = 5;
    bool g_accumulate = 1;
	int g_sampler = 0;
  }
  
  struct Tutorial : public SceneLoadingTutorialApplication
  {
    Tutorial()
      : SceneLoadingTutorialApplication("pathtracer",FEATURE_RTCORE)
    {
      registerOption("spp", [] (Ref<ParseStream> cin, const FileName& path) {
          g_spp = cin->getInt();
        }, "--spp <int>: sets number of samples per pixel");

      registerOption("max-path-length", [] (Ref<ParseStream> cin, const FileName& path) {
          g_max_path_length = cin->getInt();
        }, "--max-path-length <int>: sets maximal path length (1=primary+shadow)");

      registerOption("accumulate", [] (Ref<ParseStream> cin, const FileName& path) {
          g_accumulate = cin->getInt();
        }, "--accumulate <bool>: accumulate samples (on by default)");
    }
    
    void postParseCommandLine() 
    {
      /* load default scene if none specified */
      if (scene->size() == 0 && sceneFilename.ext() == "") {
        FileName file = FileName::executableFolder() + FileName("models/cornell_box.ecs");
        parseCommandLine(new ParseStream(new LineCommentFilter(file, "#")), file.path());
      }
    }

    void drawGUI()
    {
      ImGui::Checkbox("accumulate",&g_accumulate);
      ImGui::DragInt("##max_path_length",&g_max_path_length,1.0f,1,16, "max_path_length=%.0f");
      ImGui::DragInt("##spp",&g_spp,1.0f,1,16,"spp=%.0f");
	  ImGui::Text("sampler");
	  const char* items[] = { "default", "blue_noise" };
	  const char* current_item = items[g_sampler];
	  if (ImGui::BeginCombo("##sampler", current_item)) // The second parameter is the label previewed before opening the combo.
	  {
		  for (int i = 0; i < IM_ARRAYSIZE(items); i++)
		  {
			  bool is_selected = (current_item == items[i]); // You can store your selection however you want, outside or inside your objects
			  if (ImGui::Selectable(items[i], is_selected))
			  {
				g_sampler = i;
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
			  }
		  }
		  ImGui::EndCombo();
	  }
    }
  };

}

int main(int argc, char** argv) {
  return embree::Tutorial().main(argc,argv);
}
