#include "CImg.h"
#include "kf/kf_vector.h"
#include "kf/kf_ray.h"
#include "windows.h"
#include "vector"
#include "iostream"

using namespace cimg_library;

// The resolution of the window and the output of the ray tracer. This can be overridden by the Lua startup script.
int g_windowWidth = 1024;
int g_windowHeight = 1024;

class Circle
{
public:
	kf::Vector2 m_pos;
	float m_radius;
};

std::vector<Circle> g_circles0;	//Top Left
std::vector<Circle> g_circles1;	//Top Right
std::vector<Circle> g_circles2;	//Bottom Left
std::vector<Circle> g_circles3;	//Bottom Right


int main(int argc, char **argv)
{
	srand(0);
	// The floating point image target that the scene is rendered into.
	CImg<float> image(g_windowWidth, g_windowHeight, 1, 3, 0);
	// The display object used to show the image.
	CImgDisplay main_disp(image, "Spatial");
	main_disp.set_normalization(0);	// Normalisation 0 disables auto normalisation of the image (scales to make the darkest to brightest colour fit 0 to 1 range.

	for (int i = 0; i < 1000; ++i)
	{
		Circle c;
		c.m_pos.x = rand() % g_windowWidth;
		c.m_pos.y = rand() % g_windowHeight;
		c.m_radius = rand() % 50;

		float hwidth = g_windowWidth / 2;
		float hheight = g_windowHeight / 2;

		//Check if it's in the top left quadrant
		if(c.m_pos.x < hwidth && c.m_pos.y < hheight)
		{
			g_circles0.push_back(c);

			//Calculate overlaps.
			if (c.m_pos.x + c.m_radius > hwidth)
			{
				g_circles1.push_back(c);
			}

			if(c.m_pos.y + c.m_radius > hheight)
			{
				g_circles2.push_back(c);
			}

			if ((c.m_pos.x + c.m_radius > hwidth) && (c.m_pos.y + c.m_radius > hheight))
			{
				g_circles3.push_back(c);
			}
		}	
		//Top Right Quad
		else if (c.m_pos.x >= hwidth && c.m_pos.y < hheight)
		{
			g_circles1.push_back(c);

			//Calculate overlaps.
			if (c.m_pos.x - c.m_radius < hwidth)
			{
				g_circles0.push_back(c);
			}

			if (c.m_pos.y + c.m_radius > hheight)
			{
				g_circles3.push_back(c);
			}

			if ((c.m_pos.x - c.m_radius < hwidth) && (c.m_pos.y + c.m_radius > hheight))
			{
				g_circles2.push_back(c);
			}
		}
		//Bottom Left Quad
		else if (c.m_pos.x < hwidth && c.m_pos.y >= hheight)
		{
			g_circles2.push_back(c);

			//Calculate overlaps.
			if (c.m_pos.x + c.m_radius > hwidth)
			{
				g_circles3.push_back(c);
			}

			if (c.m_pos.y - c.m_radius < hheight)
			{
				g_circles0.push_back(c);
			}

			if ((c.m_pos.x + c.m_radius > hwidth) && (c.m_pos.y - c.m_radius < hheight))
			{
				g_circles1.push_back(c);
			}
		}
		//Bottom Right Quad
		else if (c.m_pos.x >= hwidth && c.m_pos.y >= hheight)
		{
			g_circles3.push_back(c);

			//Calculate overlaps.
			if (c.m_pos.x - c.m_radius < hwidth)
			{
				g_circles2.push_back(c);
			}

			if (c.m_pos.y - c.m_radius < hheight)
			{
				g_circles1.push_back(c);
			}

			if ((c.m_pos.x - c.m_radius < hwidth) && (c.m_pos.y - c.m_radius < hheight))
			{
				g_circles0.push_back(c);
			}
		}
	}


	// Record the starting time.
	DWORD startTime = timeGetTime();

	//Top Left Loop
	for (int y = 0; y < g_windowHeight / 2; ++y) 
	{
		for (int x = 0; x < g_windowWidth / 2; ++x) 
		{
			kf::Colour output(0, 0, 0, 0);
			float e = 0;
			for (int i = 0; i < g_circles0.size(); ++i)
			{
				//float dx = abs(g_circles[i].m_pos.x - x);
				//float dy = abs(g_circles[i].m_pos.y - y);

				//if ((dx *dx) + (dy*dy) <= g_circles[i].m_radius * g_circles[i].m_radius)
				//{
					float d = (kf::Vector2(x, y) - g_circles0[i].m_pos).length();
					if (d < g_circles0[i].m_radius)
					{
						e = e + (1.0 - d / g_circles0[i].m_radius);
					}
				//}
			}
			output.set(e, e, e, 1);

			// Clamp the output colour to 0-1 range before conversion.
			output.saturate();
			// Convert from linear space to sRGB.
			output.toSRGB();
			// Write the colour to the image (scaling up by 255).
			*image.data(x, y, 0, 0) = output.r * 255;
			*image.data(x, y, 0, 1) = output.g * 255;
			*image.data(x, y, 0, 2) = output.b * 255;
		}
		std::cout << y << std::endl;
		// Check for Escape key.
		if (main_disp.is_keyESC())
			return 0;
	}

	//Top Right Loop
	for (int y = 0; y < g_windowHeight / 2; ++y)
	{
		for (int x = g_windowWidth / 2; x < g_windowWidth; ++x)
		{
			kf::Colour output(0, 0, 0, 0);
			float e = 0;
			for (int i = 0; i < g_circles1.size(); ++i)
			{
				//float dx = abs(g_circles[i].m_pos.x - x);
				//float dy = abs(g_circles[i].m_pos.y - y);

				//if ((dx *dx) + (dy*dy) <= g_circles[i].m_radius * g_circles[i].m_radius)
				//{
				float d = (kf::Vector2(x, y) - g_circles1[i].m_pos).length();
				if (d < g_circles1[i].m_radius)
				{
					e = e + (1.0 - d / g_circles1[i].m_radius);
				}
				//}
			}
			output.set(e, e, e, 1);

			// Clamp the output colour to 0-1 range before conversion.
			output.saturate();
			// Convert from linear space to sRGB.
			output.toSRGB();
			// Write the colour to the image (scaling up by 255).
			*image.data(x, y, 0, 0) = output.r * 255;
			*image.data(x, y, 0, 1) = output.g * 255;
			*image.data(x, y, 0, 2) = output.b * 255;
		}
		std::cout << y << std::endl;
		// Check for Escape key.
		if (main_disp.is_keyESC())
			return 0;
	}

	//Bottom Left Loop
	for (int y = g_windowHeight / 2; y < g_windowHeight; ++y)
	{
		for (int x = 0; x < g_windowWidth / 2; ++x)
		{
			kf::Colour output(0, 0, 0, 0);
			float e = 0;
			for (int i = 0; i < g_circles2.size(); ++i)
			{
				//float dx = abs(g_circles[i].m_pos.x - x);
				//float dy = abs(g_circles[i].m_pos.y - y);

				//if ((dx *dx) + (dy*dy) <= g_circles[i].m_radius * g_circles[i].m_radius)
				//{
				float d = (kf::Vector2(x, y) - g_circles2[i].m_pos).length();
				if (d < g_circles2[i].m_radius)
				{
					e = e + (1.0 - d / g_circles2[i].m_radius);
				}
				//}
			}
			output.set(e, e, e, 1);

			// Clamp the output colour to 0-1 range before conversion.
			output.saturate();
			// Convert from linear space to sRGB.
			output.toSRGB();
			// Write the colour to the image (scaling up by 255).
			*image.data(x, y, 0, 0) = output.r * 255;
			*image.data(x, y, 0, 1) = output.g * 255;
			*image.data(x, y, 0, 2) = output.b * 255;
		}
		std::cout << y << std::endl;
		// Check for Escape key.
		if (main_disp.is_keyESC())
			return 0;
	}

	//Bottom Right Loop
	for (int y = g_windowHeight / 2; y < g_windowHeight; ++y)
	{
		for (int x = g_windowHeight / 2; x < g_windowWidth; ++x)
		{
			kf::Colour output(0, 0, 0, 0);
			float e = 0;
			for (int i = 0; i < g_circles3.size(); ++i)
			{
				//float dx = abs(g_circles[i].m_pos.x - x);
				//float dy = abs(g_circles[i].m_pos.y - y);

				//if ((dx *dx) + (dy*dy) <= g_circles[i].m_radius * g_circles[i].m_radius)
				//{
				float d = (kf::Vector2(x, y) - g_circles3[i].m_pos).length();
				if (d < g_circles3[i].m_radius)
				{
					e = e + (1.0 - d / g_circles3[i].m_radius);
				}
				//}
			}
			output.set(e, e, e, 1);

			// Clamp the output colour to 0-1 range before conversion.
			output.saturate();
			// Convert from linear space to sRGB.
			output.toSRGB();
			// Write the colour to the image (scaling up by 255).
			*image.data(x, y, 0, 0) = output.r * 255;
			*image.data(x, y, 0, 1) = output.g * 255;
			*image.data(x, y, 0, 2) = output.b * 255;
		}
		std::cout << y << std::endl;
		// Check for Escape key.
		if (main_disp.is_keyESC())
			return 0;
	}

	// Record ending time.
	DWORD endTime = timeGetTime();

	// Display elapsed time in the window title bar.
	main_disp.set_title("Render time: %dms", endTime - startTime);
	main_disp.display(image);

	// Keep refreshing the window until it is closed or escape is hit.
	while (!main_disp.is_closed())
	{
		if (main_disp.is_keyESC())
			return 0;
		main_disp.wait();
	}

	return 0;

}
