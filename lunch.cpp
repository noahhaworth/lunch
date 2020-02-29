#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

int collectSize();
string * collectData(int,int, string**);
string * findTags(string**,int&);
int menu();
void Random(string**, int, string*, int, string*, int, int&);
void addOption(string**, int&);
void removeOption(string**, int);
void addTag(string**, int, string*, int);
void removeTag(string**, int, string*, int);
void editName(string**, int);
void editTag(string**, int);
void clearFile(string, string);
void clear(string**, string*, string*);

string * selectedNames(int&);
string cleanString(string);
string * breakDownString(string, int&);
string * selectedNamesF(string**, int, string*, int, int&);

void checkF();
bool tagCheck(string, string);
void run(string*, int, int&);

int commaNumber(string);
bool tagCheck(string, string);
string removeString(string, string);

void showFullList(string**, int);
void showSelectedTags(string*, int);
void showSelectedNames(string*, int);
bool is_digits(const string&);
string removeComma(string);
int subMenu();
void subMain();
void showBackups();
void showBackups2();

int main()
{
	cout << endl << "********PROGRAM STARTING********" << endl << endl;
	checkF();
	int go = 1, cycle = 0, penalty = 1; 
	while (go)
		{
			if(cycle)
				cout << "****************************" << endl;
			int size = collectSize(), selectedTagSize, selectedNameSize;
			string ** list = new string *[2];
			for (int i = 0; i < 2; i++)
				list[i] = collectData(size, i, list);
			cout << endl;
			string * selectedTags = findTags(list,selectedTagSize);
			string * selectedNames = selectedNamesF(list, size, selectedTags, selectedTagSize, selectedNameSize);
			showFullList(list, size);
			showSelectedTags(selectedTags, selectedTagSize);
			showSelectedNames(selectedNames, selectedNameSize);
			switch (menu())
			{
			case 1:
				if(!size)
					break;
				Random(list, size, selectedTags, selectedTagSize, selectedNames, selectedNameSize, penalty); 
				break;
			case 2:
				addOption(list, size);
				break;
			case 3:
				removeOption(list, size);
				break;
			case 4:
				addTag(list, size, selectedTags, selectedTagSize);
				break;
			case 5:
				removeTag(list, size, selectedTags, selectedTagSize);
				break;
			case 6:
				editName(list, size);
				break;
			case 7:
				editTag(list, size);
				break;
			case 8:
				clearFile("selectedTags.txt", "selected tags");
				break;
			case 9:
				go = 0;
				break;
			case 1897:
				subMain();
				break;
			default: 
				cout << "Not a vaild selection, try again" << endl << endl;
				break;
			}
			clear(list, selectedTags, selectedNames);
			cycle++;
		}
	cout << "*********PROGRAM ENDING*********" << endl << endl;
	exit(1);
}

void showFullList(string ** list, int size)
{
	cout << "Full list:" << endl << endl;
	if(list[0][0].empty() || list[1][0].empty() || !size)
		cout << "  " << "No names input" << endl;
	else
		for(int i = 0; i < size; i++)
			cout << "  " <<list[0][i] << " (" << list[1][i] << ")" << endl;
	cout << endl;
}

void showSelectedTags(string * selectedTags, int selectedTagSize)
{
	cout << "Selected tags:" << endl << endl;
	if(selectedTags[0].empty())
		cout << "  " << "No tags selected" << endl;
	else
		for(int i = 0; i < selectedTagSize; i++)
			cout << "  " << selectedTags[i] << endl;
	cout << endl;
}

void showSelectedNames(string * selectedNames, int selectedNameSize)
{
	cout << "Selected names:" << endl << endl;
	if(!selectedNameSize)
		cout << "  " << "No selected names" << endl;
	else
		for(int i = 0; i < selectedNameSize; i++)
			cout << "  " << selectedNames[i] << endl;
}

int collectSize()
{
	string tempName, tempTag;
	int nameSize = 0, tagSize = 0;
	ifstream names, tag;
	ofstream nameSave, tagSave;
	names.open("names.txt"); tag.open("tag.txt");
	nameSave.open("nameSave.txt"); tagSave.open("tagSave.txt");
	while (getline(names, tempName) && getline(tag, tempTag))
	{
		nameSize++;
		if(tempName != "")
		{
			nameSave << tempName << endl;
			tagSave << tempTag << endl;
		}
	}
	if (getline(names,tempName)||getline(tag,tempTag))
	{
		cout << "Your files are messed up." << endl;
		exit(0);
	}
	names.close(); tag.close(); nameSave.close(); tagSave.close();
	return nameSize;
}

string * collectData(int size, int j, string** list)
{
	string file;
	if (j == 0)
		file = "nameSave.txt";
	if (j == 1)
		file = "tagSave.txt";
	string * temp = new string[size], solo;
	ifstream fin;
	fin.open(file);
	for (int i = 0; i < size; i++)
	{
		getline(fin, solo);
		temp[i] = cleanString(solo);
	}
	fin.close();
	return temp;
}

string * findTags(string ** list, int & size)
{
	size = 0;
	string temp;
	ifstream collect;
	collect.open("selectedTags.txt");
	getline(collect,temp);
	string * tags = breakDownString(cleanString(temp),size);
	collect.close();
	ofstream save; save.open("selectedTags.txt"); save << cleanString(temp); save.close();
	return tags;
}

int menu()
{
	string selection, password;
	ifstream get; get.open("password.txt"); get >> password;
	cout << endl;
	cout << "***********MENU*************" << endl << endl;
	cout << "  Select restaurant     <1>:" << endl;
	cout << "  Add restaurant        <2>:" << endl;
	cout << "  Remove restaurant     <3>:" << endl;
	cout << "  Add selected tags     <4>:" << endl;
	cout << "  Remove selected tags  <5>:" << endl;
	cout << "  Edit name             <6>:" << endl;
	cout << "  Edit tag              <7>:" << endl;
	cout << "  Clear selected tags   <8>:" << endl;
	cout << "  End program           <9>:" << endl << endl;
	cout << "Enter: ";
	getline(cin, selection);
	cout << endl;
	if(selection ==  password)
		return 1897;
	if(selection == "")
		return 0;
	if(is_digits(selection))
		return stoi(selection);
	else 
		return 0;
}

void Random(string** list, int bigSize, string * selectedTags, int size, string * selectedNames, int selectedNameSize, int & penalty)
{
	if(list[0][0].empty() || list[1][0].empty() || !bigSize)
		return;
	int allCheck = 0;
	for(int i = 0; i < size; i++)
		if(selectedTags[i] == "all" || selectedTags[i] == "ALL" || selectedTags[i] == "All")
			allCheck++;
	if(allCheck || !selectedNameSize)
	{
		run(list[0], bigSize, penalty);
		return;
	}
	run(selectedNames, selectedNameSize, penalty);
}
	

void addOption(string ** list, int& size)
{
	string name, tag;
	showFullList(list,size);
	cout << "New restaurant: ";
	getline(cin, name);
	cout << endl;
	if (name == "")
		return;
	cout << "New tags: ";
	getline(cin, tag);
	cout << endl;
	if (tag == "")
		tag = "blank";
	ofstream saveN, saveT;
	saveN.open("names.txt"); saveT.open("tag.txt");
	for (int i = 0; i < size; i++)
	{
		saveN << list[0][i] << endl;
		saveT << list[1][i] << endl;
	}
	saveN << name << endl; saveT << cleanString(tag) << endl;
	saveT.close(); saveN.close();
}

void removeOption(string ** list, int size)
{
	showFullList(list, size);
	string remove;
	int found = 0;
	cout << "Remove: ";
	getline(cin, remove);
	cout << endl;
	if(remove == "")
		return;
	for (int i = 0; i < size; i++)
		if (remove == list[0][i])
			found++;
	if (!found)
	{
		cout << endl << remove << " was not found" << endl << endl;
		removeOption(list, size);
	}
	ofstream saveN, saveT;
	saveN.open("names.txt"); saveT.open("tag.txt");
	for (int i = 0; i < size; i++)
		if (list[0][i] != remove && list[0][i] != "")
		{
			saveN << list[0][i] << endl;
			saveT << list[1][i] << endl;
		}
	saveN.close(); saveT.close();
}

void addTag(string ** list, int size, string * selectedTags, int selectedTagSize)
{
	int additionNumber;
	string additions;
	ofstream save;
	showSelectedTags(selectedTags,selectedTagSize);
	cout << "Tags to add: ";
	getline(cin,additions);
	cout << endl;
	additions = cleanString(additions);
	if(additions == "")
	{
		cout << "no additions" << endl;
		return;
	}
	string * newAdditions = breakDownString(additions,additionNumber);
	save.open("selectedTags.txt",ios::app);
	for(int i = 0; i < additionNumber; i++)
		save << "," << newAdditions[i];
	save.close();
	delete[] newAdditions;
}

void removeTag(string ** list, int size, string * selectedTags, int selectedTagSize)
{
	string removeTags;
	int found = 0, removeNumber;
	showSelectedTags(selectedTags,selectedTagSize);
	cout << "Tag to remove: ";
	getline(cin,removeTags);
	if(removeTags == "")
		return;
	removeTags = cleanString(removeTags);
	string * tooRemove = breakDownString(removeTags,removeNumber);
	for(int i = 0; i < selectedTagSize; i++)
		for(int j = 0; j < removeNumber; j++)
			if(selectedTags[i] == tooRemove[j])
				found++;
	if(!found)
	{
		cout << endl << "tags was not found" << endl << endl;
		delete[] tooRemove;
		removeTag(list,size,selectedTags,selectedTagSize);
	}
	for(int i = 0;  i < selectedTagSize; i++)
	{
		int check = 0;
		for(int j = 0;  j < removeNumber; j++ )
			if(selectedTags[i] == tooRemove[j])
				check++;
		if(check)
			selectedTags[i] = "";
	}
	delete[] tooRemove;
	ofstream save;
	save.open("selectedTags.txt");
	for(int i = 0; i < selectedTagSize; i++)
		if(selectedTags[i] != "")
			save << selectedTags[i] << ",";
	save.close();
}

void editName(string ** list, int size)
{
	string name, newName;
	int found = 0,location;
	showFullList(list, size);
	cout << "Name to edit: ";
	getline(cin, name);
	cout << endl;
	if(name == "")
		return;
	for (int i = 0; i < size; i++)
		if(name == list[0][i])
			{
				found++;
				location = i;
			}
	if(!found)
	{
		cout << endl << name << " was not found" << endl << endl;
		return;
	}
	cout << "New name: ";
	getline(cin, newName);
	if(newName == "")
		return;
	list[0][location] = newName;
	ofstream update;
	update.open("names.txt");
	for(int i = 0; i < size; i++)
		update << list[0][i] << endl;
	update.close();
}

void editTag(string ** list, int size)
{
	char selection;
	string name, newTags;
	int found = 0,location;
	showFullList(list, size);
	cout << "Name to edit: ";
	getline(cin, name);
	cout << endl;
	for (int i = 0; i < size; i++)
		if(name == list[0][i])
			{
				found++;
				location = i;
			}
	if(!found)
	{
		cout << endl << name << " was not found" << endl << endl;
		return;
	}
	cout << "Would you like to add tags or remove tags. <A/R>: ";
	cin >> selection; 
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	switch (selection)
	{
		case 'A': case 'a':
			{
				cout << "Tags to add: ";
				getline(cin, newTags);
				cout << endl;
				if(newTags == "")
					newTags = "blank";
				list[1][location] = removeString(list[1][location] + "," + cleanString(newTags), "blank");
				break;
			}
		case 'R': case 'r': 
			{
				cout << "tags to remove: ";
				getline(cin, newTags);
				cout << endl;
				int removeSize, tagSize;
				string * tooRemove = breakDownString(cleanString(newTags), removeSize), * tempTags = breakDownString(list[1][location], tagSize), replace = ",";
				for(int j = 0; j < tagSize; j++)
				{
					int check = 0;
					for(int i = 0; i < removeSize; i++)
						if(tempTags[j] == tooRemove[i])
							check++;
					if(!check)
						replace = replace + "," + tempTags[j];
				}
				replace = cleanString(replace);
				if(replace == "")
					replace = "blank";
				ofstream save; save.open("tag.txt");
				list[1][location] = replace;
				for(int i = 0; i < size; i++)
					save << list[1][i] << endl;
				save.close();
				delete[] tooRemove; delete[] tempTags;
				break;
			}
		default:
			break;
	}
	ofstream update;
	update.open("tag.txt");
	for(int i = 0; i < size; i++)
		update << list[1][i] << endl;
	update.close();
}

void clearFile(string file, string name)
{
	string check;
	cout << "Confirm to clear " << name << " <yes>: ";
	getline(cin, check);
	if(check == "yes" || check == "YES")
	{
		ofstream clear;
		clear.open(file); clear.close(); 
	}	
}

void clear(string ** list, string * selectedTags, string * selectedNames)
{
	for (int i = 0; i < 2; i++)
		delete[] list[i];
	delete[] list;
	delete[] selectedTags;
	delete[] selectedNames;
}

string * selectedNamesF(string ** list, int size, string * selectedTags, int selectedTagSize, int & selectedNameSize)
{
	selectedNameSize = 0;
	for(int i = 0; i < size; i++)
	{
		int tempSize, check = 0;
		string * tempTags = breakDownString(list[1][i], tempSize);
		for(int j = 0; j < selectedTagSize; j++)
			for(int k = 0; k < tempSize; k++)
				if(selectedTags[j] == tempTags[k])
					check++;
		if(check)
			selectedNameSize++;
		delete[] tempTags;
	}
	string * selectedNames = new string[selectedNameSize];
	int tempSize, g = 0;
	for(int i = 0; i < size; i++)
	{
		string * tempTags = breakDownString(list[1][i], tempSize);
		for(int j = 0; j < selectedTagSize; j++)
			for(int k = 0; k < tempSize; k++)
				if(selectedTags[j] == tempTags[k])
				{
					selectedNames[g] = list[0][i];
					g++; j = selectedTagSize; k = tempSize;
				}
		delete[] tempTags;		
	}
	return selectedNames;
}

void checkF()
{
	ifstream test; string name[8];
	name[0] = "names.txt"; name[1] = "nameSave.txt"; name[2] = "save.txt"; name[3] = "selectedTags.txt";
	name[4] = "selectedTagSave.txt"; name[5] = "string.txt"; name[6] = "tag.txt"; name[7] = "tagSave.txt";
	for(int i = 0; i < 8; i++)
	{
		test.open(name[i]);
		if(test.fail())
		{
			cout << name[i] << " failed to open" << endl;
			exit(0);
		}
		test.close();
	}
}

bool tagCheck(string tags, string selected)
{
	int size, found = 0;
	string * options = breakDownString(cleanString(tags),size);
	for(int i = 0; i < size; i++)
		if(selected == options[i])
			found++;
	delete[] options;
	if(found)
		return true;
	return false;
}

int commaNumber(string list)
{
	int size = list.size(), count = 0;
	for(int i = 0; i < size; i++)
		if(list[i] == ',')
			count++;
	return count;
}

string removeString(string Long, string Short)
{
	if(Long == Short)
		return "blank";
	if(Long.find(Short) != string::npos)
		Long = Long.erase(Long.find(Short)) + Long.substr(Long.find(Short) + Short.size());
	return Long;
}

string cleanString(string list)
{
	while(list[0] == ' ' || list[0] == ',')
		list = list.substr(1, list.back());
	while(list.back() == ' ' || list.back() == ',')
		list.pop_back();
	return removeComma(list);
}

string * breakDownString(string list, int & tagNumber)
{
	tagNumber = 1 + commaNumber(list);
	string * newList = new string[tagNumber];
	if(!commaNumber(list))
	{
		newList[0]=list;
		return newList;
	}
	for(int i = 0; i < tagNumber; i++)
	{
		newList[i] = list.substr(0,list.find(','));
		list = list.substr(list.find(',') + 1, list.back());
	}

	return newList;
}

void run(string* list, int bigSize, int & penalty)
{
	int pick = rand() % bigSize, addition, mutiplier;
	ifstream get; get.open("penalty.txt"); get >> addition; get >> mutiplier; get.close();
	cout << endl << "    !!!! " << list[pick] << " !!!!" << endl << endl;
	cin.ignore(penalty);
	penalty = penalty * mutiplier + addition;
}

bool is_digits(const string &str)
{
	return str.find_first_not_of("0123456789") == string::npos;
}

void subMain()
{
	int run = 1;
	string name; ifstream get; get.open("owner.txt"); get >> name; get.close();
	cout << "  Welcome " << name << endl << endl;
	while(run)
	{
		switch(subMenu())
		{
			case 1:
			{
				string check; cout << "Confirm to purge name/tag files <yes>: "; getline(cin, check); cout << endl;
				if(check == "yes" || check == "YES")
				{
					ofstream purge;
					purge.open("names.txt"); purge.close();
					purge.open("tag.txt"); purge.close();
					cout << "  Name and tag files purged" << endl << endl;
				}
				break;
			}
			case 2:
			{
				showBackups();
				string check; cout << "Confirm to backup name and tag files,\nthis will overwrite any older backups" << endl;
				cout << endl << "Do you wish to continue? <yes>: "; getline(cin, check); cout << endl;
				if(check == "yes" || check == "YES")
				{
					ifstream get; ofstream save;
					string temp;
					get.open("names.txt"); save.open("nameBackup.txt");
					while(getline(get,temp))
						save << temp << endl;
					get.close(); save.close();
					get.open("tag.txt"); save.open("tagBackup.txt");
					while(getline(get,temp))
						save << temp << endl;
					get.close(); save.close();	
					cout << "Backups complete" << endl << endl;				
				}
				break;
			}
			case 3:
			{
				showBackups2();
				string check; cout << "Confirm to backup selected tag files,\nthis will overwrite any older backup";
				cout << endl << endl << "Do you wish to continue? <yes>: "; getline(cin, check); cout << endl;
				if(check == "yes" || check == "YES")
				{
					ifstream get; ofstream save;
					string temp;
					get.open("selectedTags.txt"); save.open("selectedTagsBackup.txt");
					while(getline(get,temp))
						save << temp << endl;
					get.close(); save.close();
					cout << "Backup complete" << endl << endl;				
				}
				break;
			}
			case 4:
			{
				showBackups();
				cout << "WARNING: attempting to restore this backup will erase your current files,\n";
				cout << "and all will be lost if have not in fact backed up your data previously.\n";
				cout << "Please continue with caution." << endl << endl;
				string check; cout << "Do you wish to continue? <yes>: "; getline(cin, check); cout << endl;
				if(check == "yes" || check == "YES")
				{
					ifstream get; ofstream save;
					string temp;
					get.open("nameBackup.txt"); save.open("names.txt");
					while(getline(get, temp))
						save << temp << endl;
					get.close(); save.close();
					get.open("tagBackup.txt"); save.open("tag.txt");
					while(getline(get, temp))
						save << temp << endl;
					get.close(); save.close();	
					cout << "Restore complete, good luck" << endl << endl;				
				}
				break;
			}
			case 5:
			{
				showBackups2();
				cout << "WARNING: attempting to restore this backup will erase your current files,\n";
				cout << "and all will be lost if have not in fact backed up your data previously.\n";
				cout << "Please continue with caution." << endl << endl;
				string check; cout << "Do you wish to continue? <yes>: "; getline(cin, check); cout << endl;
				if(check == "yes" || check == "YES")
				{
					ifstream get; ofstream save;
					string temp;
					get.open("selectedTagsBackup.txt"); save.open("selectedTags.txt");
					while(getline(get,temp))
						save << temp << endl;
					get.close(); save.close();
					cout << "Restore complete, good luck" << endl << endl;				
				}
				break;
			}
			case 6:
			{
				string temp;
				int mutiplier, addition;
				ifstream get; get.open("penalty.txt"); get >> addition >> mutiplier; get.close(); 
				cout << "Current addition: " << addition << endl << endl;
				cout << "New addition: ";
				getline(cin, temp); cout << endl;
				if(temp == "")
					break;
				if(is_digits(temp))
				{
					addition = stoi(temp);
					ofstream save; save.open("penalty.txt"); save << addition << endl << mutiplier; save.close();
				}
				break;
			}

			case 7:
			{
				string temp;
				int mutiplier, addition;
				ifstream get; get.open("penalty.txt"); get >> addition >> mutiplier; get.close(); 
				cout << "Current mutiplier: " << mutiplier << endl << endl;
				cout << "New mutiplier: ";
				getline(cin, temp); cout << endl;
				if(temp == "")
					break;
				if(is_digits(temp))
				{
					mutiplier = stoi(temp);
					ofstream save; save.open("penalty.txt"); save << addition << endl << mutiplier; save.close();
				}
				break;
			}
			case 8:
			{
				string password;
				cout << "New password: ";
				getline(cin, password); cout << endl;
				if(password == "")
					break;
				ofstream save; save.open("password.txt"); save << password; save.close();
				break;
			}
			case 9:
			{
				run = 0;
				break;
			}
			case 10:
			{
				string name;
				cout << "Enter new name: ";
				getline(cin, name); cout << endl;
				if(name != "")
				{
					ofstream save; save.open("owner.txt");
					save << name; save.close();
				}
				break;
			}
			default:
			{
				cout << "Not a vaild selection, try again" << endl << endl;
				break;
			}
		}
	}
}

void showBackups()
{
	string temp1, temp2;
	ifstream name, tag; name.open("nameBackup.txt"); tag.open("tagBackup.txt");
	cout << "Back up files: " << endl << endl;
	int test = 0;
	while(getline(name, temp1) && getline(tag, temp2))
		{
			cout << "  " << temp1 << " (" << temp2 << ")"  << endl;
			test++;
		}
		if(!test)
			cout << "  No backups saved" << endl;
	name.close(); tag.close(); cout << endl;
}

int subMenu()
{
	string selection;
	cout << "**********Sub Menu**********" << endl << endl;
	cout << "  Purge names & tags    <1>:" << endl;
	cout << "  Backup names & tags   <2>:" << endl;
	cout << "  Backup selected tags  <3>:" << endl;
	cout << "  Restore names & tags  <4>:" << endl;
	cout << "  Restore selected tags <5>:" << endl;
	cout << "  Annoyance addition    <6>:" << endl;
	cout << "  Annoyance mutiplier   <7>:" << endl;
	cout << "  Change password       <8>:" << endl;
	cout << "  Exit                  <9>:" << endl;
	cout << "  Change your name     <10>:" << endl << endl;	
	cout << "Enter: ";
	getline(cin, selection);
	cout << endl;
	if(selection == "")
		return 0;
	if(is_digits(selection))
		return stoi(selection);
	else 
		return 0;
}

string removeComma(string check)
{
	char old =  check[check.size() - 1], current;
	for(int i = check.size() - 2 ; i > -1; i--)
	{
		current = check[i];
		if(current == ',' && old == ',')
			check = check.substr(0, i) + check.substr(i + 2, check.size() - 1);
	}
	return check;
}

void showBackups2()
{
	string temp;
	ifstream tag; tag.open("selectedTagsBackup.txt");
	cout << "Back up file: " << endl << endl;
	int test = 0;
	while(getline(tag, temp))
	{
		cout << "  " << temp  << endl;
		test++;
	}
	if(!test)
		cout << "  No backup file saved" << endl;
	tag.close(); cout << endl;
}