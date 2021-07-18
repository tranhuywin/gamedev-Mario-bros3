//#include "main.h"
#include <vector>
#include <iostream>
#include<fstream>

#define GRID_WIDTH	250
#define GRID_HEIGHT  250

#define BBOX_WIDTH		16
#define BBOX_HEIGHT		16
#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_FIRE_BULLET			4
#define OBJECT_TYPE_GROUND				5
#define OBJECT_TYPE_LINE				6
#define OBJECT_TYPE_TUBE				7
#define OBJECT_TYPE_QUESTION_BRICK		8
#define OBJECT_TYPE_WOODEN_BRICK		9
#define OBJECT_TYPE_FIRE_PIRANHA_PLANT	10
#define OBJECT_TYPE_ITEM				11
#define OBJECT_TYPE_TREE				12
#define OBJECT_TYPE_CARD				13
#define OBJECT_TYPE_FLYING_WOOD			14
#define OBJECT_TYPE_BROTHER				15

#define OBJECT_TYPE_PORTAL				50

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_TITLE_MAP			7
#define SCENE_SECTION_EFFECT			8
#define SCENE_SECTION_STATUS_BAR		9

#define MAX_SCENE_LINE					1024
using namespace std;
vector<string> split(string line, string delimeter = "\t")
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}
void _ParseSection_OBJECTS(string line) {

}
int main() {
	string sceneFilePath = "D:\\Project Game\\gamedev-intro-tutorials-master\\05-ScenceManager\\Grid\\New folder\\scene.txt";
	string OutPut = "D:\\Project Game\\gamedev-intro-tutorials-master\\05-ScenceManager\\Grid\\New folder\\Grid_scene.txt";
	int id = -1;

	ifstream fs;
	ofstream ofs;
	ofs.open(OutPut, ios::out);
	fs.open(sceneFilePath, ios::in);

	int section = -1;

	if (fs.fail())
	{
		cout << "Load file scene fail";
		return 0;
	}
	else {
		char str[MAX_SCENE_LINE];
		
		while (fs.getline(str, MAX_SCENE_LINE))
		{
			string line(str);
			if (line == "") continue;
			if (line[0] == '#') continue;	// skip comment lines	
			if (line == "[OBJECTS]") {
				section = SCENE_SECTION_OBJECTS; continue;
			}
			//else { section = SCENE_SECTION_UNKNOWN; continue; }

			switch (section) {
			case SCENE_SECTION_OBJECTS: {
				vector<string> tokens = split(line);
				int Left, Top, Right, Bottom;
				id++;
				//ofs << id << "\t";
				int idobj = atoi(tokens[0].c_str());
				Left = atoi(tokens[1].c_str());
				Top = atoi(tokens[2].c_str());
				int BBoxWidth = BBOX_WIDTH;
				int BBoxHeight = BBOX_HEIGHT;

				if (tokens.size() > 4)
					BBoxWidth = atoi(tokens[4].c_str());

				Right = Left + BBoxWidth;
				Bottom = Top + BBoxHeight;

				//Done export file + BBOX
				int ColStart = int(Left / GRID_WIDTH);
				int RowStart = int(Top / GRID_HEIGHT);
				int ColEnd = (int)ceil(Right / GRID_WIDTH);
				int RowEnd = (int)ceil(Bottom / GRID_HEIGHT);

				switch (idobj)
				{
				case OBJECT_TYPE_GOOMBA:
					if (ColStart > 0)
						ColStart--;
					ColEnd++;
					RowEnd++;
					break;
				case OBJECT_TYPE_KOOPAS:
					ColStart = 0;
					ColEnd++;
					RowEnd++;
					break;
				case OBJECT_TYPE_TUBE:
					RowEnd++;
					break;
				}

				if (idobj != 0)
					ofs << id << "\t" << ColStart << "\t" << RowStart << "\t" << ColEnd << "\t" << RowEnd << "\t" << idobj << "\n";
				else
					id--;
				} break;
			}
		}
	}
	
	fs.close();
	ofs.close();
	return 0;
}
