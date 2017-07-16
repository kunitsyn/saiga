#pragma once
// ImGui GLFW binding with OpenGL3 + shaders
// In this binding, ImTextureID is used to store an OpenGL 'GLuint' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui
#include "saiga/opengl/opengl.h"

#ifdef SAIGA_USE_GLFW
#include <saiga/glfw/glfw_eventhandler.h>
#include <saiga/imgui/imgui.h>

struct GLFWwindow;

namespace Saiga {

class SAIGA_GLOBAL ImGui_GLFW_Renderer : public glfw_KeyListener, public glfw_MouseListener{
protected:
    // Data
    static GLFWwindow*  g_Window;
    static double       g_Time;
    static bool         g_MousePressed[3];
    static float        g_MouseWheel ;
    static GLuint       g_FontTexture;

	//a compiler bug in vs2015 prevents multiple static variables to be declared in one line
	static int          g_ShaderHandle;
	static int  g_VertHandle;
	static int  g_FragHandle;
	static int          g_AttribLocationTex;
	static int  g_AttribLocationProjMtx;
	static int          g_AttribLocationPosition;
	static int g_AttribLocationUV;
	static int g_AttribLocationColor;
	static unsigned int g_VboHandle;
	static unsigned int g_VaoHandle;
	static unsigned int g_ElementsHandle;

    static void ImGui_ImplGlfwGL3_RenderDrawLists(ImDrawData *draw_data);
    static const char *ImGui_ImplGlfwGL3_GetClipboardText();
    static void ImGui_ImplGlfwGL3_SetClipboardText(const char *text);
    bool ImGui_ImplGlfwGL3_CreateFontsTexture();
public:
//    bool isFocused = false;
    bool wantsCaptureMouse = false;

    bool        init(GLFWwindow* window, std::string font, float fontSize = 15.0f);
    void        shutdown();

    void        checkWindowFocus();
    void        beginFrame();
    void        endFrame();

    // Use if you want to reset your rendering device without losing ImGui state.
    void        ImGui_ImplGlfwGL3_InvalidateDeviceObjects();
    bool        ImGui_ImplGlfwGL3_CreateDeviceObjects();


    bool key_event(GLFWwindow* window, int key, int scancode, int action, int mods) override;
    bool character_event(GLFWwindow* window, unsigned int codepoint) override;
    bool cursor_position_event(GLFWwindow* window, double xpos, double ypos) override;
    bool mouse_button_event(GLFWwindow* window, int button, int action, int mods) override;
    bool scroll_event(GLFWwindow* window, double xoffset, double yoffset) override;
};

}
#endif
