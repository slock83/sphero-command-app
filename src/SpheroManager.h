/*************************************************************************
	SpheroManager  -  Sphero control application -- Sphero list manager
							 -------------------
	started                : 08/05/2015
*************************************************************************/

#ifndef SPHEROMANAGER_H
#define SPHEROMANAGER_H

//-------------------------------------------------------- System includes
#include <string>
#include <vector>
#include <Sphero.hpp>

using namespace std;

class CommandHandler;
#include "CommandHandler.h"

//------------------------------------------------------- Class definition
class SpheroManager
{
	public:
		//----------------------------------------------------- Operators

		SpheroManager& operator=(const SpheroManager& sphMan) = delete;


		//--------------------------------------- Constructors/Destructor

		SpheroManager(CommandHandler *ch);

		SpheroManager(const SpheroManager& sphMan) = delete;

		virtual ~SpheroManager();


		//------------------------------------------------ Public methods

		static SpheroManager* getManager();

		/**
		 * @brief listSpheros : Lists the currently connected Spheros
		 */
		vector<string> listSpheros();

		/**
		 * @brief getSelectedIndex : Returns the index of the currently selected Sphero
		 * @return -1 if No Sphero is selected, the Sphero index in list otherwise
		 */
		unsigned int getSelectedIndex();


		/**
		 * @brief connectSphero : Connects a new Sphero into the application
		 * @param address : The Sphero bluetooth address
		 */
		bool connectSphero(string address, string name);


		/**
		 * @brief selectSphero : Selects a Sphero to command
		 * @param spheroIndex : The index of the Sphero to activate in the list
		 */
		void selectSphero(unsigned int spheroIndex);


		/**
		 * @brief disconnectSphero : Disconnects and removes from the list a Sphero
		 * @param spheroIndex : The index of the Sphero to disconnect in the list
		 */
		void disconnectSphero(unsigned int spheroIndex);


		/**
		 * @brief getSphero : Returns the currently selected Sphero
		 * @return The selected Sphero
		 */
		Sphero* getSphero();


		/**
		 * @brief getSpheroAt : Returns the Sphero at the specified index
		 * @return The Sphero locted at index
		 */
		Sphero* getSpheroAt(int index);


		/**
		 * @brief getSpheroIndex : Gets the index of a given sphero
		 * @param name : The Sphero's name
		 * @return the index of this sphero, or 0 if it doesn't exist
		 */
		int getSpheroIndex(string name);

		/**
		 * @brief getNbSpheros : Returns the number of active Spheros
		 */
		int getNbSpheros();

	private:
		Sphero* s;
		size_t nbActif;
		vector<Sphero*> spheroVec;
		vector<string> spheroNames;

		CommandHandler *_ch;
};

#endif // SPHEROMANAGER_H
