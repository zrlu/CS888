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
    int g_spp = 1;
	int g_spp_option = 0;
    int g_max_path_length = 5;
    bool g_accumulate = 1;
	int g_sampler = 0;
	bool g_pause = 0;
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

	void cameraChangeCallback()
	{
		unpause();
		tot_mray = 0;
		g_pause = 0;
	}

	void unpause()
	{
		g_spp = 1;
		g_spp_option = 0;
		g_max_path_length = 5;
	}

    void drawGUI()
    {
	  if (ImGui::Button("Render Frame"))
	  {
		g_pause = 1;
		if (!g_accumulate)
		{
			tot_mray = 0;
		}
	    doRender();
	  }
	  ImGui::SameLine();
	  if (ImGui::Button(g_pause ? "Unpause" : "Pause"))
	  {
		  g_pause = !g_pause;
		  if (g_pause)
		  {
			  unpause();
		  }
	  }
	  ImGui::SameLine();
	  if (ImGui::Button("Reset Camera"))
	  {
		  camera = camera_init;
	  }
      ImGui::Checkbox("accumulate",&g_accumulate);
	  if (ImGui::DragInt("##max_path_length", &g_max_path_length, 1.0f, 1, 16, "max_path_length=%.0f"))
	  {
		  g_pause = 1;
	  }
	  //if (ImGui::DragInt("##spp", &g_spp, 1.0f, 1, 256, "spp=%.0f"))
	  //{
		 // g_pause = 1;
	  //}

	  const int spp_options[] = { 1, 2, 4, 16, 32, 64, 128, 256 };
	  if (ImGui::BeginCombo("##spp", ("spp=" + std::to_string(spp_options[g_spp_option])).c_str()))
	  {
		  for (int i = 0; i < IM_ARRAYSIZE(spp_options); i++)
		  {
			  bool is_selected = (spp_options[g_spp_option] == g_spp);
			  if (ImGui::Selectable(std::to_string(spp_options[i]).c_str(), is_selected))
			  {
				g_spp_option = i;
			    g_spp = spp_options[i];
				g_pause = 1;
			  }
		  }
		  ImGui::EndCombo();
	  }


	  ImGui::Text("sampler");

	  const char* samplers[] = { "default", "blue_noise" };
	  const char* cur_sampler = samplers[g_sampler];

	  if (ImGui::BeginCombo("##sampler", cur_sampler)) // The second parameter is the label previewed before opening the combo.
	  {
		  for (int i = 0; i < IM_ARRAYSIZE(samplers); i++)
		  {
			  bool is_selected = (cur_sampler == samplers[i]); // You can store your selection however you want, outside or inside your objects
			  if (ImGui::Selectable(samplers[i], is_selected))
			  {
				g_sampler = i;
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
