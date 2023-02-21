#include "GUI.h"

#include <iostream>

#include "imgui/imgui.h"
#include "globalThings.h"
#include "JSONPersitence.h"
#include "imgui/imGuIZMO.quat/imGuIZMOquat.h"
bool createNew = false;
int curPhysObj = 0;
int sensitivity = 3;
float sens[5] = { 0.00005f,0.0001f,0.0005f,0.001f, 0.01f };
bool settings = false;
void GUI::render()
{
	//setup ui structure
	ImGui::Begin("Main Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Press ESC to open/close the menu at any time.");
	ImGui::Text("Sensitivity"); ImGui::SameLine();
	ImGui::SliderInt("##sensitivity", &sensitivity, 1, 5, "%0");
	rotateSpeed = sens[sensitivity];

	int current_item = 0;
	int modelSize = g_pMeshObjects.size();

	ImGui::RadioButton("Camera", &theEditMode, eEditMode::MOVING_CAMERA); ImGui::SameLine();
	ImGui::RadioButton("Lights", &theEditMode, eEditMode::MOVING_LIGHT); ImGui::SameLine();
	ImGui::RadioButton("Models", &theEditMode, eEditMode::MOVING_SELECTED_OBJECT); ImGui::SameLine();
	ImGui::RadioButton("PhysicsTest", &theEditMode, eEditMode::PHYSICS_TEST);
	if (ImGui::Button("Save"))
		persistence::SaveAll(g_pMeshObjects, g_pTheLightManager->vecTheLights);
	ImGui::SameLine();
	if (ImGui::Button("Load"))
		persistence::LoadAll(g_pMeshObjects, g_pTheLightManager->vecTheLights);
	if (theEditMode == eEditMode::MOVING_LIGHT)
	{
		if (ImGui::BeginCombo("##combo", g_pTheLightManager->vecTheLights[currentLight].name.c_str())) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < g_pTheLightManager->vecTheLights.size(); n++)
			{
				if (g_pTheLightManager->vecTheLights[n].name == "")
					continue;
				bool is_selected = (current_item == n); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(g_pTheLightManager->vecTheLights[n].name.c_str(), is_selected))
					currentLight = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		ImGui::PushItemWidth(200);
		// ATTENUATION
		glm::vec4 atten = g_pTheLightManager->vecTheLights[currentLight].atten;
		atten *= 1000;
		ImGui::SliderFloat("AttenX", &atten.x, 0, 1000.f, "%.0f");
		ImGui::SameLine(); ImGui::SliderFloat("AttenY", &atten.y, 0, 1000.f, "%.0f");
		ImGui::SameLine(); ImGui::SliderFloat("AttenZ", &atten.z, 0, 1000.f, "%.0f");
		ImGui::SliderFloat("AttenW", &atten.w, 0, 1000.f, "%.0f");
		atten /= 1000;
		g_pTheLightManager->vecTheLights[currentLight].atten = atten;
		// DIFFUSE
		glm::vec4 diff = g_pTheLightManager->vecTheLights[currentLight].diffuse;
		diff *= 100;
		ImGui::SliderFloat("DiffX", &diff.x, 0, 100.f, "%.0f");
		ImGui::SameLine(); ImGui::SliderFloat("DiffY", &diff.y, 0, 100.f, "%.0f");
		ImGui::SameLine(); ImGui::SliderFloat("DiffZ", &diff.z, 0, 100.f, "%.0f");
		diff /= 100;
		g_pTheLightManager->vecTheLights[currentLight].diffuse = diff;
		// SPECULAR
		glm::vec4 spec = g_pTheLightManager->vecTheLights[currentLight].specular;
		spec *= 100;
		ImGui::SliderFloat("SpecX", &spec.x, 0, 100.f, "%.0f");
		ImGui::SameLine(); ImGui::SliderFloat("SpecY", &spec.y, 0, 100.f, "%.0f");
		ImGui::SameLine(); ImGui::SliderFloat("SpecZ", &spec.z, 0, 100.f, "%.0f");
		spec /= 100;
		g_pTheLightManager->vecTheLights[currentLight].specular = spec;
		int lightType = g_pTheLightManager->vecTheLights[currentLight].param1.x;
		ImGui::RadioButton("PointLight", &lightType, 0); ImGui::SameLine();
		ImGui::RadioButton("SpotLight", &lightType, 1); ImGui::SameLine();
		ImGui::RadioButton("Directional", &lightType, 2);
		g_pTheLightManager->vecTheLights[currentLight].param1.x = (float)lightType;
		//float innerAngle = g_pTheLightManager->vecTheLights[currentLight].param1.y * 100;
		ImGui::SliderFloat("InnerAngle", &g_pTheLightManager->vecTheLights[currentLight].param1.y, 0.f, 100.f, "%.00f");
		//g_pTheLightManager->vecTheLights[currentLight].param1.y = innerAngle / 100;
		ImGui::SameLine(); ImGui::SliderFloat("OuterAngle", &g_pTheLightManager->vecTheLights[currentLight].param1.z, 1.f, 1000.f, "%.00f");
		int onOff = g_pTheLightManager->vecTheLights[currentLight].param2.x;
		ImGui::RadioButton("On", &onOff, 1); ImGui::SameLine();
		ImGui::RadioButton("Off", &onOff, 0); 
		g_pTheLightManager->vecTheLights[currentLight].param2.x = onOff;
		if (lightType == 1)
		{
			ImGui::Text(("x: " + std::to_string(g_pTheLightManager->vecTheLights[currentLight].direction.x) + " y: " + std::to_string(g_pTheLightManager->vecTheLights[currentLight].direction.y) + " z: " + std::to_string(g_pTheLightManager->vecTheLights[currentLight].direction.z)).c_str());
			quat direction = quat(g_pTheLightManager->vecTheLights[currentLight].qDirection.w, g_pTheLightManager->vecTheLights[currentLight].qDirection.x,
				g_pTheLightManager->vecTheLights[currentLight].qDirection.y, g_pTheLightManager->vecTheLights[currentLight].qDirection.z);
			if(ImGui::gizmo3D("Direction", direction, IMGUIZMO_DEF_SIZE, imguiGizmo::mode3Axes))
			{
				g_pTheLightManager->vecTheLights[currentLight].qDirection = glm::quat(direction.w, direction.x, direction.y, direction.z);
				glm::mat4 matRotation = glm::mat4(g_pTheLightManager->vecTheLights[currentLight].qDirection);
				glm::vec4 eulerDir = glm::vec4(0.f, 0.f, -1.f, 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[currentLight].direction = eulerDir;
			}
		}
	}

	//if (theEditMode == eEditMode::MOVING_PHYSICS_OBJECT)
	//{
	//	if (ImGui::BeginCombo("##combo", g_physics_system.m_PhysicsObjects[curPhysObj]->GetName().c_str())) // The second parameter is the label previewed before opening the combo.
	//	{
	//		for (int n = 0; n < g_physics_system.m_PhysicsObjects.size(); n++)
	//		{
	//			bool is_selected = (current_item == n); // You can store your selection however you want, outside or inside your objects
	//			if (ImGui::Selectable(g_physics_system.m_PhysicsObjects[n]->GetName().c_str(), is_selected))
	//				curPhysObj = n;
	//			if (is_selected)
	//				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
	//		}
	//		ImGui::EndCombo();
	//	}
	//	ImGui::PushItemWidth(200);
	//	ImGui::Text("Position: "); ImGui::SameLine();
	//	ImGui::SliderFloat("posX", &g_physics_system.m_PhysicsObjects[curPhysObj]->position.x, -500.0f, 500.0f, "%.00f");
	//	ImGui::SameLine();
	//	ImGui::SliderFloat("posY", &g_physics_system.m_PhysicsObjects[curPhysObj]->position.y, -500.0f, 500.0f, "%.00f");
	//	ImGui::SameLine();
	//	ImGui::SliderFloat("posZ", &g_physics_system.m_PhysicsObjects[curPhysObj]->position.z, -500.0f, 500.0f, "%.00f");
	//}
	if (theEditMode == eEditMode::MOVING_SELECTED_OBJECT)
	{
		if (ImGui::BeginCombo("##combo", g_pMeshObjects[currentModel]->friendlyName.c_str())) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < modelSize; n++)
			{
				bool is_selected = (current_item == n); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(g_pMeshObjects[n]->friendlyName.c_str(), is_selected))
					currentModel = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		ImGui::PushItemWidth(200);
		ImGui::Text("Position: "); ImGui::SameLine();
		ImGui::SliderFloat("posX", &g_pMeshObjects[currentModel]->position.x, -500.0f, 500.0f, "%.00f");
		ImGui::SameLine();
		ImGui::SliderFloat("posY", &g_pMeshObjects[currentModel]->position.y, -500.0f, 500.0f, "%.00f");
		ImGui::SameLine();
		ImGui::SliderFloat("posZ", &g_pMeshObjects[currentModel]->position.z, -500.0f, 500.0f, "%.00f");

		ImGui::Text("Rotation: "); ImGui::SameLine();
		ImGui::PushItemWidth(50); ImGui::InputFloat("rX", &g_pMeshObjects[currentModel]->qRotation.x);  ImGui::SameLine(); ImGui::PushItemWidth(200);
		ImGui::SliderFloat("rotX", &g_pMeshObjects[currentModel]->qRotation.x, -500.0f, 500.0f, "%.00f");
		ImGui::SameLine();
		ImGui::PushItemWidth(50); ImGui::InputFloat("rY", &g_pMeshObjects[currentModel]->qRotation.y);  ImGui::SameLine(); ImGui::PushItemWidth(200);
		ImGui::SliderFloat("rotY", &g_pMeshObjects[currentModel]->qRotation.y, -500.0f, 500.0f, "%.00f");
		ImGui::SameLine();
		ImGui::PushItemWidth(50); ImGui::InputFloat("rZ", &g_pMeshObjects[currentModel]->qRotation.z);  ImGui::SameLine(); ImGui::PushItemWidth(200);
		ImGui::SliderFloat("rotZ", &g_pMeshObjects[currentModel]->qRotation.z, -500.0f, 500.0f, "%.00f");
		ImGui::Text("Scale: "); ImGui::SameLine();
		float scale;
		scale = (g_pMeshObjects[currentModel]->scaleXYZ.x + g_pMeshObjects[currentModel]->scaleXYZ.y + g_pMeshObjects[currentModel]->scaleXYZ.z) / 3;
		ImGui::PushItemWidth(50); ImGui::InputFloat("TotalScale", &scale); ImGui::SameLine();
		if (ImGui::Button("Apply"))
		{
			g_pMeshObjects[currentModel]->SetUniformScale(scale);
		}
		ImGui::InputFloat("sX", &g_pMeshObjects[currentModel]->scaleXYZ.x);  ImGui::SameLine(); ImGui::PushItemWidth(200);
		ImGui::SliderFloat("sclX", &g_pMeshObjects[currentModel]->scaleXYZ.x, -500.0f, 500.0f, "%.00f");
		ImGui::SameLine();
		ImGui::PushItemWidth(50); ImGui::InputFloat("sY", &g_pMeshObjects[currentModel]->scaleXYZ.y);  ImGui::SameLine(); ImGui::PushItemWidth(200);
		ImGui::SliderFloat("sclY", &g_pMeshObjects[currentModel]->scaleXYZ.y, -500.0f, 500.0f, "%.00f");
		ImGui::SameLine();
		ImGui::PushItemWidth(50); ImGui::InputFloat("sZ", &g_pMeshObjects[currentModel]->scaleXYZ.z);  ImGui::SameLine(); ImGui::PushItemWidth(200);
		ImGui::SliderFloat("sclZ", &g_pMeshObjects[currentModel]->scaleXYZ.z, -500.0f, 500.0f, "%.00f");
		ImGui::PushItemWidth(1000);
		float transparency = g_pMeshObjects[currentModel]->RGBA_colour.w * 100;
		ImGui::SliderFloat("Transparency", &transparency, 1.f, 100.0f, "%.00f");
		g_pMeshObjects[currentModel]->RGBA_colour.w = transparency / 100;
		ImGui::Text("WireFrame: "); ImGui::SameLine();
		ImGui::Checkbox("##wireframe", &g_pMeshObjects[currentModel]->isWireframe);



		quat qRotation = quat(g_pMeshObjects[currentModel]->qRotation.w,g_pMeshObjects[currentModel]->qRotation.x, g_pMeshObjects[currentModel]->qRotation.y, g_pMeshObjects[currentModel]->qRotation.z);
		if(ImGui::gizmo3D("##gizmo1", qRotation, 200, imguiGizmo::modeFullAxes | imguiGizmo::cubeAtOrigin))
		{
			g_pMeshObjects[currentModel]->qRotation = glm::quat(qRotation.w,qRotation.x, qRotation.y, qRotation.z);
			if(g_pMeshObjects[currentModel]->friendlyName == "Beholder1")
			{
				glm::mat4 matRotation = glm::mat4(g_pMeshObjects[currentModel]->qRotation);
				glm::vec4 eulerDir = glm::vec4(angles[0], 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[7].direction = eulerDir;
				matRotation = glm::mat4(g_pMeshObjects[currentModel]->qRotation);
				eulerDir = glm::vec4(angles[1] , 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[8].direction = eulerDir;
				matRotation = glm::mat4(g_pMeshObjects[currentModel]->qRotation);
				eulerDir = glm::vec4(angles[2], 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[9].direction = eulerDir;
			}
			if (g_pMeshObjects[currentModel]->friendlyName == "Beholder2")
			{
				glm::mat4 matRotation = glm::mat4(g_pMeshObjects[currentModel]->qRotation);
				glm::vec4 eulerDir = glm::vec4(angles[0], 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[10].direction = eulerDir;
				matRotation = glm::mat4(g_pMeshObjects[currentModel]->qRotation);
				eulerDir = glm::vec4(angles[1], 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[11].direction = eulerDir;
				matRotation = glm::mat4(g_pMeshObjects[currentModel]->qRotation);
				eulerDir = glm::vec4(angles[2], 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[12].direction = eulerDir;
			}
			if (g_pMeshObjects[currentModel]->friendlyName == "Beholder3")
			{
				glm::mat4 matRotation = glm::mat4(g_pMeshObjects[currentModel]->qRotation);
				glm::vec4 eulerDir = glm::vec4(angles[0], 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[11].direction = eulerDir;
				matRotation = glm::mat4(g_pMeshObjects[currentModel]->qRotation);
				eulerDir = glm::vec4(angles[1], 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[12].direction = eulerDir;
				matRotation = glm::mat4(g_pMeshObjects[currentModel]->qRotation);
				eulerDir = glm::vec4(angles[2], 1.f);
				eulerDir = matRotation * eulerDir;
				g_pTheLightManager->vecTheLights[13].direction = eulerDir;
			}
		}//glm::vec4(0.096419f, -0.480074f, -0.871918, 1.f)


		//g_pMeshObjects[currentModel]->qRotation = glm::quat(qRotation.x, qRotation.y, qRotation.z, qRotation.w);
		if (ImGui::Button("Make Copy"))
		{
			createNew = !createNew;
			isTyping = !isTyping;
		}
		if (createNew)
		{
			static char name[64] = "";
			cMeshObject* obj = new cMeshObject;
			*obj = *g_pMeshObjects[currentModel];
			ImGui::InputText("Name", name, 64);
			if (ImGui::Button("Create"))
			{
				obj->friendlyName = name;
				pVAOManager->AddObject(*obj);
				g_pMeshObjects.push_back(obj);
				modelSize++;
				isTyping = false;
				createNew = false;
			}
		}
	}

	ImGui::End();
}
void render(const std::string& sound)
{
	ImGui::Begin((sound + " Radio Settings").c_str());
	{
	}
	ImGui::End();
}