#include "Player.h"
#include "Entity.h"
#include "World.h"
#include "Exit.h"

Player::Player(const char* name, const char* description, Room* parent, Item* inventory) : Entity(name, description, parent),
	currentRoom(parent),
	inventory(inventory)
	{
		type = PLAYER;
	}

Player::~Player(){}

void Player::Look(p2Vector<p2String>& commands) const{
	Item* aux;
	int j = 0;
	printf("\n----%s----", parent->getName());
	printf("\n%s\n", parent->getDescription());
	for (int i = 0; i < myWorld->entities.size(); i++){
		if (myWorld->entities[i]->getType() == ITEM){
			aux = dynamic_cast<Item*> (myWorld->entities[i]);
			if (aux->GetContainer() == currentRoom){
				if (j == 0){
					printf("These are the objects I can see: ");
					j++;
				}
				printf("\n\t- %s ", aux->getName());
			}
		}
	}

	printf("\n");
}

void Player::Go(p2Vector<p2String>& commands){
	if (commands.size() > (uint)1){
		Exit* aux = currentRoom->GetExit(commands[1]);
		if (aux != nullptr && aux->getName() == commands[1]){
			if (!aux->get_Close()){
				parent = aux->get_Destination();
				currentRoom = aux->get_Destination();
			}
			else {
				printf("Door is closed\n");
			}
		}
		else printf("There is no room in direction %s.\n", commands[1].c_str());
	}
	else {
		Exit* aux = currentRoom->GetExit(commands[0]);
		if (aux != nullptr && aux->getName() == commands[0]){
			if (!aux->get_Close()){
				parent = aux->get_Destination();
				currentRoom = aux->get_Destination();
			}
			else {
				printf("Door is closed\n");
			}
		}
		else printf("There is no room in direction %s.\n", commands[0].c_str());
	}
}

void Player::ChangeDoor(p2Vector<p2String>& commands){
	Exit* departure;
	Exit* ret = nullptr;

	departure = currentRoom->GetExit(commands[1]);
	if (departure != nullptr){
		for (int i = 0; i < myWorld->entities.size(); i++){
			if (myWorld->entities[i]->getType() == EXIT){
				ret = dynamic_cast<Exit*>(myWorld->entities[i]);
				if (ret->get_Origin() == departure->get_Destination() && ret->get_Destination() == departure->get_Origin()) break;
				else ret = nullptr;
			}
		}
		if (ret != nullptr){
			if (commands[0] == "close"){
				if (departure->get_Close()){
					departure->CloseDoor();
					ret->CloseDoor();
					printf("The door is closed now.\n");
				}
			}
			else if (commands[0] == "open"){
				departure->OpenDoor();
				ret->OpenDoor();
				printf("The door is open now.\n");
			}
		}
	}
	else printf("I cant find a door in %s direction.\n", commands[1]);

}

void Player::Pick(p2Vector<p2String>& commands){
	Item* temp;
	bool picked = false;
	if (commands.size() == (uint)2){
		for (int i = 0; i < myWorld->entities.size(); i++){
			if (myWorld->entities[i]->getType() == ITEM){
				temp = dynamic_cast<Item*> (myWorld->entities[i]);
				if (temp->GetContainer() == currentRoom && temp->getName() == commands[1] && temp->GetPickable()){
					temp->SetContainer(inventory);
					picked = true;
					break;
				}
			}
		}
	}
	if (picked) printf("I've put the %s on my %s.\n", commands[1].c_str(), inventory->getName());
	else printf("There is not an item named %s here.\n", commands[1].c_str());
}