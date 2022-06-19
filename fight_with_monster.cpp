#include <iostream>
#include <array>
#include <ctime>
#include <random>
#include <string_view>

//fungsi random ini untuk menentukan monster secara acak dan dipakai untuk menentukan > 50 atau dibawahnya
int get_random_numbers(int min, int max){
   std::mt19937_64 twister {static_cast<std::mt19937_64::result_type>(std::time(nullptr))};
   std::uniform_int_distribution<int> random_number(min, max);
   return random_number(twister);
}

class Creature{
protected:
   std::string m_name;
   std::string m_symbol;
   int m_health;
   int m_demage;
   int m_gold;
public:
   Creature(std::string_view name, std::string_view symbol, int health, int demage, int gold) : m_name{name}, m_symbol{symbol}, m_health{health}, m_demage{demage}, m_gold{gold}{}
   Creature() = default;

   //getter
   std::string_view get_name()const{return m_name;}
   std::string_view get_symbol(){
      if(m_name == "dragon"){
         m_symbol += "\U0001F409";
     }else if(m_name == "orc"){
         m_symbol += "\U0001F47A";
     }else{
         m_symbol += "\U0001F5FF";
     }
      return m_symbol;
   }
   int get_health()const{return m_health;}
   int get_demage()const{return m_demage;}
   int get_gold()const{return m_gold;}
   //getter

   //additional methods
   void reduce_health(int value){
      this->m_health -= value;
   }
   bool is_dead(){
      return this->m_health <= 0;
   }
   void add_gold(int value){
      this->m_gold += value;
   }
   //additional methods
};

class Potion{
protected:
   enum class PotionType{
      HEALT_SMALL , 
      HEALT_MEDIUM,  
      HEALT_LARGE,
      STRENGTH_SMALL,  
      STRENGTH_MEDIUM,  
      STRENGTH_LARGE,  
      POISON_SMALL,
      POISON_MEDIUM,
      POISON_LARGE,
      max_size     
   }m_pt;
   int m_health_potion;
   int m_damage_potion;

public:
   Potion(int healt, int damage, PotionType pt) : m_health_potion(healt), m_damage_potion(damage), m_pt(pt){}

   static Potion random_potion(){
      //data dari look-up table tadi diacak disini, referensi mengenai apa itu static bisa dilihat di beyond_of_topics/casting
      return potion_data(static_cast<PotionType>(get_random_numbers(0, static_cast<int>(PotionType::max_size) - 1)));
   }

   const char* potion_size(){
      const char* result;
      switch(m_pt){
         case PotionType::HEALT_SMALL:
         case PotionType::STRENGTH_SMALL:
         case PotionType::POISON_SMALL:
         result = "Small";
         break;

         case PotionType::HEALT_MEDIUM:
         case PotionType::STRENGTH_MEDIUM:
         case PotionType::POISON_MEDIUM:
         result = "Medium";
         break;

         case PotionType::HEALT_LARGE:
         case PotionType::STRENGTH_LARGE:
         case PotionType::POISON_LARGE:
         result = "Large";
         break;
      }
       return result;
   }

   const char* potion_name(){
      const char* result;
      switch(m_pt){
         case PotionType::HEALT_SMALL:
         case PotionType::HEALT_MEDIUM:
         case PotionType::HEALT_LARGE:
         result = "Healt";
         break;

         case PotionType::STRENGTH_SMALL:
         case PotionType::STRENGTH_MEDIUM:
         case PotionType::STRENGTH_LARGE:
         result = "Strength";
         break;

         case PotionType::POISON_SMALL:
         case PotionType::POISON_MEDIUM:
         case PotionType::POISON_LARGE:
         result = "Poison";
         break;
      }
      return result;
   }

   int get_health_potion()const{return m_health_potion;}
   int get_damage_potion()const{return m_damage_potion;}

private:
   static Potion& potion_data(const PotionType p){
      static std::array<Potion, static_cast<int>(PotionType::max_size)>data
      {
         {
            {2,0,PotionType::HEALT_SMALL},
            {2,0,PotionType::HEALT_MEDIUM},
            {5,0,PotionType::HEALT_LARGE},
            {0,1,PotionType::STRENGTH_SMALL},
            {0,2,PotionType::STRENGTH_MEDIUM},
            {0,3,PotionType::STRENGTH_LARGE},
            {-1,0,PotionType::POISON_SMALL},
            {-2,0,PotionType::POISON_MEDIUM},
            {-5,0,PotionType::POISON_LARGE}
         }
      };
      return data.at(static_cast<int>(p));
   }
};

class Player : public Creature{
private:
   int m_player_level {1};
public:
   Player(std::string_view name) : Creature{name, "@", 10, 1, 0}{}

   void level_up(){
      ++m_player_level;
      ++m_demage;
   }
   const int& get_player_level()const{return m_player_level;}
   bool has_won(){return get_player_level() >= 20;}

   //another class
   void drink_potion(Potion& p){
      m_health += p.get_health_potion();
      m_demage += p.get_damage_potion();
   }
   //another class
};

class Monster : public Creature{
public:
   enum class Type{DRAGON,ORC,SLIME,max_types};

   Monster(Type type):Creature{get_default_creature(type)}{}

    static Monster get_random_monster(){
      int acak = get_random_numbers(0, static_cast<int>(Type::max_types) - 1);
      return Monster {static_cast<Type>(acak)};
   }
private:
   static const Creature& get_default_creature(Type t){
      static std::array<Creature, static_cast<std::size_t>(Type::max_types)> monster_stat{
         {
            //member ini hanya ada di Creature
            {"dragon","",20,4,100},
            {"orc","",4,2,25},
            {"slime","",1,1,10}
         }
      };
      return monster_stat.at(static_cast<std::size_t>(t));
   }
};


Player get_player_name(){
   std::string player_name;
   std::cout<<"Enter your name: ";
   std::cin>>player_name;
   std::cout<<"Welcome,"<<player_name<<std::endl;
   return Player {player_name};
}

//bertanggung jawab atas serangan user ke monster
void attack_monster(Player &p, Monster& m){
   std::cout<<"\nYou hit the "<<m.get_name()<<" for "<<p.get_demage()<<" damage"<<std::endl;
   m.reduce_health(p.get_demage());

   if(m.is_dead()){
      std::cout<<'\n';
      std::cout<<"===================="<<std::endl;
      p.level_up();
      std::cout<<"You killed the "<<m.get_name()<<std::endl;
      std::cout<<"You are now level "<<p.get_player_level()<<std::endl;
      std::cout<<"You found "<<m.get_gold()<<" gold"<<std::endl;
      p.add_gold(m.get_gold());

   if(get_random_numbers(1, 100) == 30){
      char drink_or_not;
      std::cout<<"You found a mythical potion! Do you want to drink it? [y/n]: ";
      std::cin>>drink_or_not;
         if(drink_or_not == 'y' || drink_or_not == 'Y'){
            Potion pt {Potion::random_potion()};
            std::cout<<"You drank a "<<pt.potion_size()<<" potion of "<<pt.potion_name()<<std::endl;
               p.drink_potion(pt);
            return;
         }else{
            std::cout<<"It's ok \U0001F44C"<<std::endl;
            return;
         }
      }
      std::cout<<"===================="<<std::endl;
   }
}

//bertanggung jawab atas serangan monster ke user
void attack_player(Player &p, Monster &m){
   std::cout<<"\nThe "<<m.get_name()<<" hit you for "<<m.get_demage()<<" damage"<<std::endl;
   p.reduce_health(m.get_demage());
}

//fungsi ini akan terus dijalankan selama user belum mati/menang
void keep_fighting(Player& p, Monster& m){
   //ambil monster secara acak, inilah salah satu keuntungan static, tidak perlu dibuat objek
   m = Monster::get_random_monster();
   std::cout<<"\nYou have encountered a "<<m.get_name()<<'('<<m.get_symbol()<<")"<<std::endl;

   std::cout<<"\n(R)un or (F)ight ";
   char r_or_f;
   std::cin>>r_or_f;

   if(r_or_f == 'F' || r_or_f == 'f'){
      //kita serang monster
      attack_monster(p,m);
      //jika monster tidak mati
      if(!m.is_dead())
         attack_player(p, m);
      return;
   }else if(r_or_f == 'R' || r_or_f == 'r'){
      //harus diatas 50 agar bisa selamat, ceritanya 50%
      if(get_random_numbers(1,100) >= 50){
         std::cout<<"You succesfully fled"<<std::endl;
         return;
      }else{
         std::cout<<"You failed to fled"<<std::endl;
         attack_player(p, m);
      }
   }
}


int main(int argc, char const **argv){
  Player player {get_player_name()};
  Monster monster {Monster::get_random_monster()};

  while(!player.has_won() && !player.is_dead()){
   keep_fighting(player, monster);
  }
  
  if (player.is_dead()){
    std::cout << "\nYou died at level " << player.get_player_level() << " and with " << player.get_gold() << " gold.\n";
    std::cout << "Too bad you can't take it with you!\n";
  }else{
    std::cout << "\nYou won the game with " << player.get_gold() << " gold!\n";
  }

   return 0;
}





