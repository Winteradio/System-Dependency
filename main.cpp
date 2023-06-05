#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <map>
#include <cstdarg>

struct ISystem
{	
	public :
		ISystem() {};
		ISystem( std::string Name ) : Name( Name ) {};
		
		virtual ~ISystem() {};
		
	public :
		void Frame()
		{
			std::cout << "Start : " << Name << std::endl;
		}
		
		std::string& GetName() { return Name; }
		
	private :
		std::string Name;
}; 


class SystemManager
{
	using Data = std::map< std::string, ISystem* >;
	using Need = std::map< std::string, std::vector< std::string > >;
	using Sequence = std::queue< ISystem* >;
	using Done = std::queue< ISystem* >;
	
	private :
		SystemManager() {};
		virtual ~SystemManager() {};
		
	public :
		static SystemManager& GetHandle()
		{  
			return m_SystemManager;
		}
		
		ISystem* Create( std::string Name )
		{
			ISystem* System = new ISystem( Name );
			
			m_Data[ Name ] = System;
			m_Need[ Name ] = std::vector< std::string >();
			
			std::cout << "Create : " << Name << std::endl;
			
			return System;
		}
		
		void SetDependency( ISystem*& Main, ISystem*& Dependency )
		{
			m_Need[ Dependency->GetName() ].push_back( Main->GetName() );
			Sort();
		}
		
		
		void Destroy()
		{
			for ( auto ITR : m_Data )
			{
				delete ITR.second;
			}
			
			m_Data.clear();
		}
		
		void Frame()
		{
			while( !m_Sequence.empty() )
			{
				ISystem*& System = m_Sequence.front();
				System->Frame();
				m_Done.push( System );
				m_Sequence.pop();
			}
			
			while( !m_Done.empty() )
			{
				m_Sequence.push( m_Done.front() );
				m_Done.pop();
			}
		}
		
		void Sort()
		{
			m_Sequence = std::queue< ISystem* >();
			
			std::map< std::string, int > DepCount;
			
			for ( auto ITR : m_Need )
			{
				std::string Need = ITR.first;		
				DepCount[ Need ] = 0;		
			}
			
			for ( auto ITR : m_Need )
			{
				std::vector< std::string > NeedsList = ITR.second;
				
				for ( auto Name : NeedsList )
				{
					DepCount[ Name ]++;
				}
			}
			
			std::cout << " " << std::endl;
			for ( auto ITR : DepCount )
			{
				std::cout << ITR.first << " : " << ITR.second << std::endl;
			}

			std::queue< ISystem* > Temp;			
			while ( m_Sequence.size() < m_Data.size() )
			{
				for ( auto ITR : DepCount )
				{
					std::string Name = ITR.first;
					int Count = ITR.second;
					
					if ( Count == 0 )
					{
						m_Sequence.push( m_Data[ Name ] );
						Temp.push( m_Data[ Name ] );
						std::cout << "Input : " << Name << std::endl;
						DepCount[ Name ]--;
					}
				}
				
				while( !Temp.empty() )
				{
					std::string Name = Temp.front()->GetName();
					for ( auto System : m_Need[ Name ] )
					{
						DepCount[ System ]--;
					}
					Temp.pop();
				}
				
				std::cout << " " << std::endl;
				for ( auto ITR : DepCount )
				{
					std::cout << ITR.first << " : " << ITR.second << std::endl;
				}
			}
		}
		
	private :
		static SystemManager m_SystemManager;
		Data m_Data;
		Need m_Need;
		Sequence m_Sequence;
		Done m_Done;
};

SystemManager SystemManager::m_SystemManager;

int main()
{
	ISystem* Render = SystemManager::GetHandle().Create( "RenderSystem" );
	ISystem* Move = SystemManager::GetHandle().Create( "MovementSystem" );
	ISystem* Collision = SystemManager::GetHandle().Create( "CollisionSystem" );
	ISystem* Light = SystemManager::GetHandle().Create( "LightSystem" );
	ISystem* Physics = SystemManager::GetHandle().Create( "PhysicsSystem" );

	SystemManager::GetHandle().SetDependency( Move, Collision );
	SystemManager::GetHandle().SetDependency( Render, Move );
	SystemManager::GetHandle().SetDependency( Render, Light );
	SystemManager::GetHandle().SetDependency( Collision, Physics );
	SystemManager::GetHandle().SetDependency( Light, Physics );
	
	SystemManager::GetHandle().Frame();
	SystemManager::GetHandle().Destroy();

	return 0;
}
