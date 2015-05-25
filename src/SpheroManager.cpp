/*************************************************************************
	SpheroManager  -  Sphero control application -- Sphero list manager
							 -------------------
	started                : 08/05/2015
*************************************************************************/

//-------------------------------------------------------- System includes

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <QDebug>

#include <sphero/bluetooth/bluez_adaptor.h>
#include <sphero/Sphero.hpp>
#include <sphero/packets/Constants.hpp>

using namespace std;

//--------------------------------------------------------- Local includes
#include "SpheroManager.h"
#include "CommandHandler.h"


//------------------------------------------------ Constructors/Destructor
/**
 * @brief SpheroManager : Constructor
 */
SpheroManager::SpheroManager(CommandHandler *ch):s(NULL), _ch(ch)
{
	spheroNames = new vector<string>();
}

SpheroManager::~SpheroManager()
{}


//--------------------------------------------------------- Public methods

/**
 * @brief listSpheros : Lists the currently connected Spheros
 */
vector<string>* SpheroManager::listSpheros()
{
	return spheroNames;
}


/**
 * @brief getSelectedIndex : Returns the index of the currently selected Sphero
 * @return -1 if No Sphero is selected, the Sphero index in list otherwise
 */
unsigned int SpheroManager::getSelectedIndex()
{
	for(unsigned int i = 0; i < spheroVec.size(); ++i)
	{
		if(spheroVec[i] == s)
			return i;
	}
	return -1;
}



/**
 * @brief connectSphero : Connects a new Sphero into the application
 * @param address : The Sphero bluetooth address
 */
bool SpheroManager::connectSphero(string address, string name)
{
	if(address.length() == 0)
	{
		ifstream myfile ("lastConnection");
		if (myfile.is_open())
		{
			getline(myfile,address);
			myfile.close();
		}

		else
		{
			_ch->setStatusBar("Unable to retrieve an address");
			return false;
		}
	}

	Sphero* sph = new Sphero(address.c_str(), new bluez_adaptor());

	sph->onDisconnect([this](){
		_ch->setStatusBar("Sphero disconnected");
	});

	/*sph->onData([sph](){
		uint16_t var;

		if(sph->getDataBuffer()->waitForNext(dataTypes::ODOMETER_X, var, -1))
			cout << "posX:" << var << endl;

		if(sph->getDataBuffer()->waitForNext(dataTypes::ODOMETER_Y, var, -1))
			cout << "posY:" << var << endl;
	});*/

	if(sph->connect())
	{
		_ch->setStatusBar("Sphero connected successfully");
		spheroVec.push_back(sph);
		spheroNames->push_back(name);

		s = sph;
		ofstream myfile ("lastConnection", ios::out);
		if (myfile.is_open())
		{
			myfile << address << " " << name;
			myfile.close();
			cerr << "Sphero address saved ! next time, just type \"connect\" :)"<< endl;
		}
		else cerr << "Error : can't save the address :("<< endl;

	}
	else
	{
		_ch->setStatusBar("Connection error");
		delete sph;
	}
	return true;
}


/**
 * @brief selectSphero : Selects a Sphero to command
 * @param spheroIndex : The index of the Sphero to activate in the list
 */
bool SpheroManager::selectSphero(unsigned int spheroIndex)
{
	if(spheroVec.size() > spheroIndex)
	{
		s = spheroVec[spheroIndex];
		return true;
	}
	return false;
}


/**
 * @brief disconnectSphero : Disconnects and removes from the list a Sphero
 * @param spheroIndex : The index of the Sphero to disconnect in the list
 */
void SpheroManager::disconnectSphero(unsigned int spheroIndex)
{
	if(spheroVec.size() > spheroIndex)
	{
		if(s == spheroVec[spheroIndex])
		{
			s = NULL;
		}

		spheroVec[spheroIndex]->disconnect();
		delete spheroVec[spheroIndex];

		spheroVec.erase(spheroVec.begin() + spheroIndex);
		spheroNames->erase(spheroNames->begin() + spheroIndex);
	}
	else
	{
		cout << "Error : invalid index" << endl;
	}
}


/**
 * @brief getSphero : Returns the currently selected Sphero
 * @return The selected Sphero
 */
Sphero* SpheroManager::getSphero()
{
	return s;
}


/**
 * @brief getSphero : Returns the Sphero with the given name
 * @param name : The name used to search the Sphero
 * @return The selected Sphero
 */
Sphero* SpheroManager::getSphero(string name)
{
	for(unsigned int i = 0; i < spheroNames->size(); ++i)
	{
		if((*spheroNames)[i] == name)
			return spheroVec[i];
	}
	return NULL;
}


/**
 * @brief getSpheroAt : Returns the Sphero at the specified index
 * @return The Sphero locted at index
 */
Sphero *SpheroManager::getSpheroAt(unsigned int index)
{
	if(index >= spheroVec.size())
		return NULL;

	return spheroVec[index];
}


/**
 * @brief getSpheroIndex : Gets the index of a given sphero
 * @param name : The Sphero's name
 * @return the index of this sphero, or 0 if it doesn't exist
 */
int SpheroManager::getSpheroIndex(string name)
{
	for(unsigned int i = 0; i < spheroNames->size(); ++i)
	{
		if(name == (*spheroNames)[i])
			return i;
	}
	return 0;
}


/**
 * @brief getNbSpheros : Returns the number of active Spheros
 */
int SpheroManager::getNbSpheros()
{
	return spheroVec.size();
}
