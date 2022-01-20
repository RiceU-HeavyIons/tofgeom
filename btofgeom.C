// tofgeom macro
// basic example of how to set up the environment to interactively access
// the BTOF geometry
// fg. 1/20'22
btofgeom(char *tag="y2017", int dbdate=20210101, int dbtime=10000){

// Baseline shared libraries
        gSystem->Load("libTable");
        gSystem->Load("St_base");
        gSystem->Load("StChain");
        gSystem->Load("StUtilities");
        gSystem->Load("St_Tables.so");
        gSystem->Load("StDbLib.so");
        gSystem->Load("StDbBroker.so");
        gSystem->Load("St_db_Maker.so");

// Load BTOF geometry library
        gSystem->Load("StBTofUtil");

// Load the STAR geometry - prepare gGeoManager pointer
        ifstream ifile;
        char buf[100];
        sprintf(buf,"%s_geometry.C", tag);
        ifile.open(buf);
        if(ifile) {
                // load geometry file if available
                TString pname(gProgName);
                if ( pname != "root4star" ) {
                        gSystem->Load("/usr/lib/mysql/libmysqlclient.so");
                }
                cout << "========== Loading "<< tag << " from pre-configured geometry file ==========" << endl;
                gROOT->Macro(buf);
        } else {
                // create geometry file
                gROOT->LoadMacro("$STAR/StarVMC/Geometry/macros/loadAgML.C");
                cout << "========== Recreating "<< tag << " geometry from AgML ==========" << endl;
                loadAgML(tag);
                cout << "========== Storing pre-configured geometry file " << buf << "==========" << endl;
                gGeoManager->Export(buf, "mygeom");
        }


// Set up database maker with approriate day/time stamp
        cout << "========== Setting up database environment timestamp=("<<dbdate << "," << dbtime <<")==========" << endl;
        St_db_Maker *dbMk = new St_db_Maker("dbName","MySQL:StarDb","$STAR/StarDb","");
        dbMk->Init();
        dbMk->SetDateTime(dbdate,dbtime);

// Set up and initialize the BTOF Geometry class
        cout << "========== Setting up BTOF Geometry Object ==========" << endl;
        StBTofGeometry *mBTofGeom = new StBTofGeometry("btofGeometry","Standalone btofGeometry");
        TVolume *starHall = dbMk->GetDataSet("HALL");
        mBTofGeom->Init(dbMk,starHall,gGeoManager);

        if (mBTofGeom->IsInitDone()) {
                mBTofGeom->Print();
                cout << "==== Use mBTofGeom to access BTOF geometry: ====" << endl;

                // A few examples:
                cout << "==== mBTofGeom->GetGeomTray(1)->Print() ====" << endl;
                mBTofGeom->GetGeomTray(1)->Print();

                cout << "==== mBTofGeom->GetGeomCell(23039)->Print() ====" << endl;
                mBTofGeom->GetGeomCell(23039)->Print();

        }
        else cerr << "=== mBTOFGEOM init FAILED ===" << endl;



} // end macro


// More examples:
// mBTofGeom->IsTrayValid(1)
// mBTofGeom->IsTrayValid(110)
// mBTofGeom->IsTrayValid(120)
// mBTofGeom->IsTrayValid(130)

// StBTofGeomSensor *mycell = mBTofGeom->GetGeomCell(1)
// mycell->GetCellYMin(1)
// mycell->GetCellYMin(2)
// mycell->GetCellYMin(6)
// (mycell->GetCellPosition(1))->x()
// (mycell->GetCellPosition(1))->y()
// (mycell->GetCellPosition(1))->z()

// StBTofGeomSensor *mycell = mBTofGeom->GetGeomCell(10000)
// (mycell->GetCellPosition(6))->z()
// (mycell->GetCellPosition(6))->x()
// (mycell->GetCellPosition(6))->y()

// mBTofGeom->GetGeomTray(120)->Print()
// mBTofGeom->GetGeomCell(1)->Print()
// mBTofGeom->GetGeomCell(0)->Print()
// mBTofGeom->GetGeomCell(10000)->Print()
// mBTofGeom->GetGeomCell(23039)->Print()
// mBTofGeom->GetGeomCell(23040)->Print()

// mBTofGeom->ModulesInTray(100)

// mBTofGeom->GetGeomCell(23039)->GetCells()
// mBTofGeom->GetGeomCell(23039)->GetCellPosition(1)
// mBTofGeom->GetGeomCell(23039)->GetCellPosition(1)->z()
// mBTofGeom->GetGeomCell(23039)->GetCellPosition(1)->pseudoRapidity()
