#pragma ident "$Id: Rinex3ObsHeader.cpp 1 2011-06-06 19:45:41Z btolman $"

//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//  
//  Copyright 2004, The University of Texas at Austin
//
//============================================================================

//============================================================================
//
//This software developed by Applied Research Laboratories at the University of
//Texas at Austin, under contract to an agency or agencies within the U.S. 
//Department of Defense. The U.S. Government retains all rights to use,
//duplicate, distribute, disclose, or release this software. 
//
//Pursuant to DoD Directive 523024 
//
// DISTRIBUTION STATEMENT A: This software has been approved for public 
//                           release, distribution is unlimited.
//
//=============================================================================

/**
 * @file Rinex3ObsHeader.cpp
 * Encapsulate header of Rinex observation file, including I/O
 */

#include <sstream>

#include "StringUtils.hpp"
#include "SystemTime.hpp"
#include "TimeString.hpp"
#include "Rinex3ObsStream.hpp"
#include "Rinex3ObsHeader.hpp"

using namespace std;
using namespace gpstk::StringUtils;

namespace gpstk
{
   const string Rinex3ObsHeader::stringVersion           = "RINEX VERSION / TYPE";
   const string Rinex3ObsHeader::stringRunBy             = "PGM / RUN BY / DATE";
   const string Rinex3ObsHeader::stringComment           = "COMMENT";
   const string Rinex3ObsHeader::stringMarkerName        = "MARKER NAME";
   const string Rinex3ObsHeader::stringMarkerNumber      = "MARKER NUMBER";
   const string Rinex3ObsHeader::stringMarkerType        = "MARKER TYPE";
   const string Rinex3ObsHeader::stringObserver          = "OBSERVER / AGENCY";
   const string Rinex3ObsHeader::stringReceiver          = "REC # / TYPE / VERS";
   const string Rinex3ObsHeader::stringAntennaType       = "ANT # / TYPE";
   const string Rinex3ObsHeader::stringAntennaPosition   = "APPROX POSITION XYZ";
   const string Rinex3ObsHeader::stringAntennaDeltaHEN   = "ANTENNA: DELTA H/E/N";
   const string Rinex3ObsHeader::stringAntennaDeltaXYZ   = "ANTENNA: DELTA X/Y/Z";
   const string Rinex3ObsHeader::stringAntennaPhaseCtr   = "ANTENNA: PHASECENTER";
   const string Rinex3ObsHeader::stringAntennaBsightXYZ  = "ANTENNA: B.SIGHT XYZ";
   const string Rinex3ObsHeader::stringAntennaZeroDirAzi = "ANTENNA: ZERODIR AZI";
   const string Rinex3ObsHeader::stringAntennaZeroDirXYZ = "ANTENNA: ZERODIR XYZ";
   const string Rinex3ObsHeader::stringCenterOfMass      = "CENTER OF MASS: XYZ";
   const string Rinex3ObsHeader::stringSystemNumObs      = "SYS / # / OBS TYPES";
   const string Rinex3ObsHeader::stringSigStrengthUnit   = "SIGNAL STRENGTH UNIT";
   const string Rinex3ObsHeader::stringInterval          = "INTERVAL";
   const string Rinex3ObsHeader::stringFirstTime         = "TIME OF FIRST OBS";
   const string Rinex3ObsHeader::stringLastTime          = "TIME OF LAST OBS";
   const string Rinex3ObsHeader::stringReceiverOffset    = "RCV CLOCK OFFS APPL";
   const string Rinex3ObsHeader::stringSystemDCBSapplied = "SYS / DCBS APPLIED";
   const string Rinex3ObsHeader::stringSystemPCVSapplied = "SYS / PCVS APPLIED";
   const string Rinex3ObsHeader::stringSystemScaleFac    = "SYS / SCALE FACTOR";
   const string Rinex3ObsHeader::stringSystemPhaseShift  = "SYS / PHASE SHIFTS";
   const string Rinex3ObsHeader::stringGlonassSlotFreqNo = "GLONASS SLOT / FRQ #";
   const string Rinex3ObsHeader::stringLeapSeconds       = "LEAP SECONDS";
   const string Rinex3ObsHeader::stringNumSats           = "# OF SATELLITES";
   const string Rinex3ObsHeader::stringPrnObs            = "PRN / # OF OBS";
   const string Rinex3ObsHeader::stringEoH               = "END OF HEADER";


   void Rinex3ObsHeader::reallyPutRecord(FFStream& ffs) const
      throw(exception, FFStreamError, StringException)
   {
      Rinex3ObsStream& strm = dynamic_cast<Rinex3ObsStream&>(ffs);

      strm.header = *this;

      unsigned long allValid;
      if      (version == 3.0)   allValid = allValid30;
      else if (version == 3.01)  allValid = allValid301;
      else
      {
         FFStreamError err("Unknown RINEX version: " + asString(version,2));
         err.addText("Make sure to set the version correctly.");
         GPSTK_THROW(err);
      }

      if ((valid & allValid) != allValid)
      {
         ostringstream msg;
         msg << endl;
         msg << "Version = " << version << endl;
         msg << "allValid30 = " << hex << setfill('0') << setw(2) << nouppercase
               << allValid30 << endl;
         msg << "allValid   = " << hex << setfill('0') << setw(2) << nouppercase
               << allValid << endl;
         msg << "   valid   = " << hex << setfill('0') << setw(2) << nouppercase
               << valid << endl;
         msg << "      OOPS." << endl;
         msg << "Version     " << (valid & validVersion        ) << endl;
         msg << "Run By      " << (valid & validRunBy          ) << endl;
         msg << "Marker Name " << (valid & validMarkerName     ) << endl;
         msg << "Marker Type " << (valid & validMarkerType     ) << endl;
         msg << "Observer    " << (valid & validObserver       ) << endl;
         msg << "Receiver    " << (valid & validReceiver       ) << endl;
         msg << "Antenna Type" << (valid & validAntennaType    ) << endl;
         msg << "Antenna DHEN" << (valid & validAntennaDeltaHEN) << endl;
         msg << "Sys Obs Type" << (valid & validSystemObsType  ) << endl;
         msg << "Sys Phs Shft" << (valid & validSystemPhaseShift)<< endl;
         msg << "GLO Freq No " << (valid & validGlonassFreqNo  ) << endl;
         msg << "Interval    " << (valid & validInterval       ) << endl;
         msg << "First Time  " << (valid & validFirstTime      ) << endl;
         msg << "End Header  " << (valid & validEoH            ) << endl;
         FFStreamError err("Incomplete or invalid header.");
         err.addText("Make sure you set all header valid bits for all of the available data.");
         err.addText(msg.str());
         GPSTK_THROW(err);
      }

      try
      {
         WriteHeaderRecords(strm);
      }
      catch(FFStreamError& e)
      {
         GPSTK_RETHROW(e);
      }
      catch(StringException& e)
      {
         GPSTK_RETHROW(e);
      }

   }  // end reallyPutRecord


   // This function computes the number of valid header records
   // which WriteHeaderRecords will write.
   int Rinex3ObsHeader::NumberHeaderRecordsToBeWritten(void) const throw()
   {
      int n = 0;

      if (valid & validVersion          ) n++;
      if (valid & validRunBy            ) n++;
      if (valid & validComment          ) n += commentList.size();
      if (valid & validMarkerName       ) n++;
      if (valid & validMarkerNumber     ) n++;
      if (valid & validMarkerType       ) n++;
      if (valid & validObserver         ) n++;
      if (valid & validReceiver         ) n++;
      if (valid & validAntennaType      ) n++;
      if (valid & validAntennaPosition  ) n++;
      if (valid & validAntennaDeltaHEN  ) n++;
      if (valid & validAntennaDeltaXYZ  ) n++;
      if (valid & validAntennaPhaseCtr  ) n++;
      if (valid & validAntennaBsightXYZ ) n++;
      if (valid & validAntennaZeroDirAzi) n++;
      if (valid & validAntennaZeroDirXYZ) n++;
      if (valid & validCenterOfMass     ) n++;
      if (valid & validSystemObsType    ) n += 1 + (obsTypeList.size()-1)/9;
      if (valid & validSigStrengthUnit  ) n++;
      if (valid & validInterval         ) n++;
      if (valid & validFirstTime        ) n++;
      if (valid & validLastTime         ) n++;
      if (valid & validReceiverOffset   ) n++;
      if (valid & validSystemDCBSapplied) n++;
      if (valid & validSystemPCVSapplied) n++;
      if (valid & validSystemScaleFac   ) n++;
      if (valid & validSystemPhaseShift ) n++;
      if (valid & validGlonassFreqNo    ) n++;
      if (valid & validLeapSeconds      ) n++;
      if (valid & validNumSats          ) n++;
      if (valid & validPrnObs           )
         n += numObsForSat.size() * (1+numObsForSat.begin()->second.size()/9);
      if (valid & validEoH              ) n++;

      return n;
   }  // end NumberHeaderRecordsToBeWritten


   // This function writes all valid header records.
   void Rinex3ObsHeader::WriteHeaderRecords(FFStream& ffs) const
      throw(FFStreamError, StringException)
   {
      Rinex3ObsStream& strm = dynamic_cast<Rinex3ObsStream&>(ffs);
      string line;


      if (valid & validVersion)
      {
         line  = rightJustify(asString(version,2), 9);
         line += string(11, ' ');

         if ((fileType[0] != 'O') && (fileType[0] != 'o'))
         {
            FFStreamError err("This isn't a RINEX Observation file: " +
                              fileType.substr(0,1));
            GPSTK_THROW(err);
         }

         if (system.system == RinexSatID::systemUnknown)
         {
            FFStreamError err("Invalid satellite system");
            GPSTK_THROW(err);
         }

         line += leftJustify(string("OBSERVATION DATA"), 20);
         string str;
         str = system.systemChar();
         str = str + " (" + system.systemString() + ")";
         line += leftJustify(str, 20);
         line += stringVersion;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validVersion" << endl;
      if (valid & validRunBy)
      {
         line  = leftJustify(fileProgram, 20);
         line += leftJustify(fileAgency , 20);
         SystemTime sysTime;
         string curDate = printTime(sysTime,"%04Y%02m%02d %02H%02M%02S %P");
         line += leftJustify(curDate, 20);
         line += stringRunBy;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validRunBy" << endl;
      if (valid & validComment)
      {
         vector<string>::const_iterator itr = commentList.begin();
         while (itr != commentList.end())
         {
            line  = leftJustify((*itr), 60);
            line += stringComment;
            strm << line << endl;
            strm.lineNumber++;
            itr++;
         }
      }
//      cout << "past validComment" << endl;
      if (valid & validMarkerName)
      {
         line  = leftJustify(markerName, 60);
         line += stringMarkerName;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validMarkerName" << endl;
      if (valid & validMarkerNumber)
      {
         line  = leftJustify(markerNumber, 20);
         line += string(40, ' ');
         line += stringMarkerNumber;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validMarkerNumber" << endl;
      if (valid & validMarkerType)
      {
         line  = leftJustify(markerType, 20);
         line += string(40, ' ');
         line += stringMarkerType;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validMarkerType" << endl;
      if (valid & validObserver)
      {
         line  = leftJustify(observer, 20);
         line += leftJustify(agency  , 40);
         line += stringObserver;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validObserver" << endl;
      if (valid & validReceiver)
      {
         line  = leftJustify(recNo  , 20);
         line += leftJustify(recType, 20);
         line += leftJustify(recVers, 20);
         line += stringReceiver;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validReceiver" << endl;
      if (valid & validAntennaType)
      {
         line  = leftJustify(antNo  , 20);
         line += leftJustify(antType, 20);
         line += string(20, ' ');
         line += stringAntennaType;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validAntennaType" << endl;
      if (valid & validAntennaPosition)
      {
         line  = rightJustify(asString(antennaPosition[0], 4), 14);
         line += rightJustify(asString(antennaPosition[1], 4), 14);
         line += rightJustify(asString(antennaPosition[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaPosition;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validAntennaPosition" << endl;
      if (valid & validAntennaDeltaHEN)
      {
         line  = rightJustify(asString(antennaDeltaHEN[0], 4), 14);
         line += rightJustify(asString(antennaDeltaHEN[1], 4), 14);
         line += rightJustify(asString(antennaDeltaHEN[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaDeltaHEN;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validAntennaDeltaHEN" << endl;
      if (valid & validAntennaDeltaXYZ)
      {
         line  = rightJustify(asString(antennaDeltaXYZ[0], 4), 14);
         line += rightJustify(asString(antennaDeltaXYZ[1], 4), 14);
         line += rightJustify(asString(antennaDeltaXYZ[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaDeltaXYZ;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validAntennaDeltaXYZ" << endl;
      if (valid & validAntennaPhaseCtr)
      {
         line  =  leftJustify(antennaSatSys , 1);
         line += string( 1, ' ');
         line += rightJustify(antennaObsCode, 3);
         line += rightJustify(asString(antennaPhaseCtr[0], 4),  9);
         line += rightJustify(asString(antennaPhaseCtr[1], 4), 14);
         line += rightJustify(asString(antennaPhaseCtr[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaPhaseCtr;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validAntennaPhaseCtr" << endl;
      if (valid & validAntennaBsightXYZ)
      {
         line  = rightJustify(asString(antennaBsightXYZ[0], 4), 14);
         line += rightJustify(asString(antennaBsightXYZ[1], 4), 14);
         line += rightJustify(asString(antennaBsightXYZ[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaBsightXYZ;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validAntennaBsightXYZ" << endl;
      if (valid & validAntennaZeroDirAzi)
      {
         line  = rightJustify(asString(antennaZeroDirAzi, 4), 14);
         line += string(46, ' ');
         line += stringAntennaZeroDirAzi;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validAntennaZeroDirAzi" << endl;
      if (valid & validAntennaZeroDirXYZ)
      {
         line  = rightJustify(asString(antennaZeroDirXYZ[0], 4), 14);
         line += rightJustify(asString(antennaZeroDirXYZ[1], 4), 14);
         line += rightJustify(asString(antennaZeroDirXYZ[2], 4), 14);
         line += string(18, ' ');
         line += stringAntennaZeroDirXYZ;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validAntennaZeroDirXYZ" << endl;
      if (valid & validCenterOfMass)
      {
         line  = rightJustify(asString(centerOfMass[0], 4), 14);
         line += rightJustify(asString(centerOfMass[1], 4), 14);
         line += rightJustify(asString(centerOfMass[2], 4), 14);
         line += string(18, ' ');
         line += stringCenterOfMass;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validCenterOfMass" << endl;
      if (valid & validSystemObsType)
      {
         static const int maxObsPerLine = 13;

         map<string,vector<ObsID> >::const_iterator mapIter;
         for (mapIter = mapObsTypes.begin(); mapIter != mapObsTypes.end(); mapIter++)
         {
            int obsWritten = 0;
            line = ""; // make sure the line contents are reset

            vector<ObsID> ObsTypeList = mapIter->second;

            for (int i = 0; i < ObsTypeList.size(); i++)
            {
               // the first line needs to have the GNSS type and # of obs
               if (obsWritten == 0)
               {
                  line  =  leftJustify(mapIter->first, 1);
                  line += string(2, ' ');
                  line += rightJustify(asString(ObsTypeList.size()), 3);
               }
               // if you hit 13, write out the line and start a new one
               else if ((obsWritten % maxObsPerLine) == 0)
               {
                  line += string(2, ' ');
                  line += stringSystemNumObs;
                  strm << line << endl;
                  strm.lineNumber++;
                  line  = string(6, ' ');
               }
               line += string(1, ' ');
               line += rightJustify(ObsTypeList[i].asRinex3ID(), 3);
               obsWritten++;
            }
            line += string(60 - line.size(), ' ');
            line += stringSystemNumObs;
            strm << line << endl;
            strm.lineNumber++;
         }
      }
//      cout << "past validSystemObsType" << endl;
      if (valid & validSigStrengthUnit)
      {
         line  = leftJustify(sigStrengthUnit, 20);
         line += string(40, ' ');
         line += stringSigStrengthUnit;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validSigStrengthUnit" << endl;
      if (valid & validInterval)
      {
         line  = rightJustify(asString(interval, 3), 10);
         line += string(50, ' ');
         line += stringInterval;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validInterval" << endl;
      if (valid & validFirstTime)
      {
         line  = writeTime(firstObs);
         line += string(60 - line.size(), ' ');
         line += stringFirstTime;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validFirstTime" << endl;
      if (valid & validLastTime)
      {
         line  = writeTime(lastObs);
         line += string(60 - line.size(), ' ');
         line += stringLastTime;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validLastTime" << endl;
      if (valid & validReceiverOffset)
      {
         line  = rightJustify(asString(receiverOffset), 6);
         line += string(54, ' ');
         line += stringReceiverOffset;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validReceiverOffset" << endl;
      if (valid & validSystemDCBSapplied)
      {
         for (int i = 0; i < infoDCBS.size(); i++)
         {
           line  = leftJustify(infoDCBS[i].satSys,  1);
           line += string(1, ' ');
           line += leftJustify(infoDCBS[i].name  , 17);
           line += string(1, ' ');
           line += leftJustify(infoDCBS[i].source, 40);
           line += stringSystemDCBSapplied;
           strm << line << endl;
           strm.lineNumber++;
         }
      }
//      cout << "past validSystemDCBSapplied" << endl;
      if (valid & validSystemPCVSapplied)
      {
         for (int i = 0; i < infoPCVS.size(); i++)
         {
           line  = leftJustify(infoPCVS[i].satSys,  1);
           line += string(1, ' ');
           line += leftJustify(infoPCVS[i].name  , 17);
           line += string(1, ' ');
           line += leftJustify(infoPCVS[i].source, 40);
           line += stringSystemPCVSapplied;
           strm << line << endl;
           strm.lineNumber++;
         }
      }
//      cout << "past validSystemPCVSapplied" << endl;
      if (valid & validSystemScaleFac)
      {
         static const int maxObsPerLine = 12;

         static const int size = 4;
         static const int factors[size] = {1,10,100,1000};
         vector<string> obsTypes;

         // loop over GNSSes
         map<string, sfacMap>::const_iterator mapIter;
         for (mapIter = sysSfacMap.begin(); mapIter != sysSfacMap.end(); mapIter++)
         {
            map<ObsID, int>::const_iterator iter;

            for (int i = 0; i < size; i++) // loop over possible factors (above)
            {
               int count = 0;
               obsTypes.clear(); // clear the list of Obs Types we're going to make

               for (iter = mapIter->second.begin();      // loop over scale factor map
                     iter != mapIter->second.end(); iter++)
               {
                  if ( iter->second == factors[i] )
                  {
                     count++;
                     obsTypes.push_back(iter->first.asRinex3ID());
                  }
               }

               if ( count == 0 ) continue;

               line  =  leftJustify(mapIter->first      , 1);
               line += string(1, ' ');
               line += rightJustify(asString(factors[i]), 4);
               line += string(2, ' ');
               line += rightJustify(asString(count     ), 2);

               for (int j = 0; j < count; j++)
               {
                  if (j > maxObsPerLine-1 && (j % maxObsPerLine) == 0 )
                  {
                  // need continuation; end current line
                     line += string(2, ' ');
                     line += stringSystemScaleFac;
                     strm << line << endl;
                     strm.lineNumber++;
                     line  = string(10, ' ');
                  }
                  line += string(1, ' ');
                  line += rightJustify(obsTypes[j], 3);
               }
               int space = 60 - 10 - 4*(count % maxObsPerLine);
               line += string(space, ' ');
               line += stringSystemScaleFac;
               strm << line << endl;
               strm.lineNumber++;
            }
         }
      }
//      cout << "past validSystemScaleFac" << endl;
      if (valid & validSystemPhaseShift)
      {
         //map<ObsID, map<RinexSatID,double> > sysPhaseShift;
         map<ObsID, map<RinexSatID,double> >::const_iterator it;
         for(it=sysPhaseShift.begin(); it!=sysPhaseShift.end(); ++it) {
            ObsID obsid(it->first);
            RinexSatID sat(it->second.begin()->first);
            double corr(it->second.begin()->second);

            line  = string(" ") + sat.systemChar();
            if(sat.id == -1) {
               line += string(60-line.length(), ' ');
               line += stringSystemPhaseShift;
               strm << line << endl;
               strm.lineNumber++;
            }
            else {                  // list of sats
               line += obsid.asRinex3ID() + string(" ");
               line += rightJustify(asString(corr,5),8);
               setfill('0');
               line += string("  ") + rightJustify(asString(it->second.size()),2);
               setfill(' ');

               int n(0);
               map<RinexSatID,double>::const_iterator jt,kt;
               for(jt=it->second.begin(); jt!=it->second.end(); ++jt) {
                  line += string(" ") + jt->first.toString();
                  if(++n == 10 || ++(kt=jt) == it->second.end()) {   // end this line
                     line += string(60-line.length(), ' ');
                     line += stringSystemPhaseShift;
                     strm << line << endl;
                     strm.lineNumber++;
                     n = 0;
                     // are there more for a continuation line?
                     if(kt != it->second.end())
                        line = string(18,' ');
                  }
               }
            }
         }
      }
//      cout << "past validSystemPhaseShift" << endl;
      if (valid & validGlonassFreqNo)
      {
         //map<RinexSatID,int> GlonassFreqNo;
         int n(0),nsat(GlonassFreqNo.size());
         line = rightJustify(asString(nsat),3) + string(" ");
         map<RinexSatID,int>::const_iterator it,kt;
         for(it = GlonassFreqNo.begin(); it != GlonassFreqNo.end(); ++it) {
            line += it->first.toString();
            line += rightJustify(asString(it->second),3);
            if(++n == 8 || ++(kt=it) == GlonassFreqNo.end()) {    // write it
               line += string(60-line.length(), ' ');
               strm << line << endl;
               strm.lineNumber++;
               n = 0;
               // are there more for a continuation line?
               if(kt != GlonassFreqNo.end())
                  line = string(4,' ');
            }
         }
      }
//      cout << "past validGlonassFreqNo" << endl;
      if (valid & validLeapSeconds)
      {
         line  = rightJustify(asString(leapSeconds), 6);
         line += string(54, ' ');
         line += stringLeapSeconds;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validLeapSeconds" << endl;
      if (valid & validNumSats)
      {
         line  = rightJustify(asString(numSVs), 6);
         line += string(54, ' ');
         line += stringNumSats;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validNumSats" << endl;
      if (valid & validPrnObs)
      {
         static const int maxObsPerLine = 9;
         map<RinexSatID, vector<int> >::const_iterator itr = numObsForSat.begin();
         while (itr != numObsForSat.end())
         {
            int numObsWritten = 0;
            
            vector<int>::const_iterator vecItr = (*itr).second.begin();
            while (vecItr != (*itr).second.end())
            {
               if (numObsWritten == 0)
               {
                  try
                  {
                     line = string(3, ' ') + (*itr).first.toString();
                  }
                  catch (Exception& e)
                  {
                     FFStreamError ffse(e);
                     GPSTK_RETHROW(ffse); 
                  }
               }
               else if ((numObsWritten % maxObsPerLine) == 0)
               {
                  line += stringPrnObs;
                  strm << line << endl;
                  strm.lineNumber++;
                  line  = string(6, ' ');
               }
               line += rightJustify(asString(*vecItr), 6);
               ++vecItr;
               ++numObsWritten;
            }
            line += string(60 - line.size(), ' ');
            line += stringPrnObs;
            strm << line << endl;
            strm.lineNumber++;
            itr++;
         }
      }
//      cout << "past validPrnObs" << endl;
      if (valid & validEoH)
      {
         line  = string(60, ' ');
         line += stringEoH;
         strm << line << endl;
         strm.lineNumber++;
      }
//      cout << "past validEoH" << endl;
//      cout << "R3ObsHead: wrote header records" << endl;
   } // end WriteHeaderRecords


   // This function parses a single header record.
   void Rinex3ObsHeader::ParseHeaderRecord(string& line)
      throw(FFStreamError)
   {
      string label(line, 60, 20);
         
      if (label == stringVersion)
      {
         version  = asDouble(line.substr( 0,20));
         fileType = strip(   line.substr(20,20));
         satSys   = strip(   line.substr(40,20));
         system.fromString(satSys);
         if ( fileType[0] != 'O' && fileType[0] != 'o')
         {
            FFStreamError e("This isn't a RINEX 3 Obs file.");
            GPSTK_THROW(e);
         }
         if ( satSys[0] != 'G' && satSys[0] != 'g' &&
              satSys[0] != 'R' && satSys[0] != 'r' &&
              satSys[0] != 'E' && satSys[0] != 'e' &&
              satSys[0] != 'S' && satSys[0] != 's' &&
              satSys[0] != 'M' && satSys[0] != 'm'    )
         {
           FFStreamError e("The satellite system isn't valid.");
           GPSTK_THROW(e);
         }
         valid |= validVersion;
      }
      else if (label == stringRunBy)
      {
         fileProgram = strip(line.substr( 0,20));
         fileAgency  = strip(line.substr(20,20));
         date        = strip(line.substr(40,20));
         valid |= validRunBy;
      }
      else if (label == stringComment)
      {
         commentList.push_back(strip(line.substr(0,60)));
         valid |= validComment;
      }
      else if (label == stringMarkerName)
      {
         markerName = strip(line.substr(0,60));
         valid |= validMarkerName;
      }
      else if (label == stringMarkerNumber)
      {
         markerNumber = strip(line.substr(0,20));
         valid |= validMarkerNumber;
      }
      else if (label == stringMarkerType)
      {
         markerType = strip(line.substr(0,20));
         valid |= validMarkerType;
      }
      else if (label == stringObserver)
      {
         observer = strip(line.substr( 0,20));
         agency   = strip(line.substr(20,40));
         valid |= validObserver;
      }
      else if (label == stringReceiver)
      {
         recNo   = strip(line.substr( 0,20));
         recType = strip(line.substr(20,20));
         recVers = strip(line.substr(40,20));
         valid |= validReceiver;
      }
      else if (label ==stringAntennaType)
      {
         antNo   = strip(line.substr( 0,20));
         antType = strip(line.substr(20,20));
         valid |= validAntennaType;
      }
      else if (label == stringAntennaPosition)
      {
         antennaPosition[0] = asDouble(line.substr( 0,14));
         antennaPosition[1] = asDouble(line.substr(14,14));
         antennaPosition[2] = asDouble(line.substr(28,14));
         valid |= validAntennaPosition;
      }
      else if (label == stringAntennaDeltaHEN)
      {
         antennaDeltaHEN[0] = asDouble(line.substr( 0,14));
         antennaDeltaHEN[1] = asDouble(line.substr(14,14));
         antennaDeltaHEN[2] = asDouble(line.substr(28,14));
         valid |= validAntennaDeltaHEN;
      }
      else if (label == stringAntennaDeltaXYZ)
      {
         antennaDeltaXYZ[0] = asDouble(line.substr( 0,14));
         antennaDeltaXYZ[1] = asDouble(line.substr(14,14));
         antennaDeltaXYZ[2] = asDouble(line.substr(28,14));
         valid |= validAntennaDeltaXYZ;
      }
      else if (label == stringAntennaPhaseCtr)
      {
         antennaSatSys  = strip(line.substr(0,2));
         antennaObsCode = strip(line.substr(2,3));
         antennaPhaseCtr[0] = asDouble(line.substr( 5, 9));
         antennaPhaseCtr[1] = asDouble(line.substr(14,14));
         antennaPhaseCtr[2] = asDouble(line.substr(28,14));
         valid |= validAntennaPhaseCtr;
      }
      else if (label == stringAntennaBsightXYZ)
      {
         antennaBsightXYZ[0] = asDouble(line.substr( 0,14));
         antennaBsightXYZ[1] = asDouble(line.substr(14,14));
         antennaBsightXYZ[2] = asDouble(line.substr(28,14));
         valid |= validAntennaBsightXYZ;
      }
      else if (label == stringAntennaZeroDirAzi)
      {
         antennaZeroDirAzi = asDouble(line.substr(0,14));
         valid |= validAntennaBsightXYZ;
      }
      else if (label == stringAntennaZeroDirXYZ)
      {
         antennaZeroDirXYZ[0] = asDouble(line.substr( 0,14));
         antennaZeroDirXYZ[1] = asDouble(line.substr(14,14));
         antennaZeroDirXYZ[2] = asDouble(line.substr(28,14));
         valid |= validAntennaBsightXYZ;
      }
      else if (label == stringCenterOfMass)
      {
         centerOfMass[0] = asDouble(line.substr( 0,14));
         centerOfMass[1] = asDouble(line.substr(14,14));
         centerOfMass[2] = asDouble(line.substr(28,14));
         valid |= validCenterOfMass;
      }
      else if (label == stringSystemNumObs)
      {
         static const int maxObsPerLine = 13;

         satSysTemp = strip(line.substr(0,1));
         numObs     = asInt(line.substr(3,3));

         if ( satSysTemp == "" ) // it's a continuation line; use previous info.
         {
            satSysTemp = satSysPrev;
            numObs = numObsPrev;
            vector<ObsID> newTypeList = mapObsTypes.find(satSysTemp)->second;
            for (int i = newTypeList.size();
                 (i < numObs) && ( (i % maxObsPerLine) < maxObsPerLine); i++)
            {
               int position = 4*(i % maxObsPerLine) + 6 + 1;
               ObsID rt(satSysTemp+line.substr(position,3));
               newTypeList.push_back(rt);
            }
            mapObsTypes[satSysTemp] = newTypeList;
         }
         else                    // it's a new line, use info. read in
         {
            vector<ObsID> newTypeList;
            for (int i = 0; (i < numObs) && (i < maxObsPerLine); i++)
            {
               int position = 4*i + 6 + 1;
               ObsID rt(satSysTemp+line.substr(position,3));
               newTypeList.push_back(rt);
            }
            mapObsTypes[satSysTemp] = newTypeList;
         }

         // save values in case next line is a continuation line
         satSysPrev = satSysTemp;
         numObsPrev = numObs;

         valid |= validSystemObsType;
      }
      else if (label == stringSigStrengthUnit)
      {
         sigStrengthUnit = strip(line.substr(0,20));
         valid |= validSigStrengthUnit;
      }
      else if (label == stringInterval)
      {
         interval = asDouble(line.substr(0,10));
         valid |= validInterval;
      }
      else if (label == stringFirstTime)
      {
         firstObs = parseTime(line);
         valid |= validFirstTime;
      }
      else if (label == stringLastTime)
      {
         lastObs = parseTime(line);
         valid |= validLastTime;
      }
      else if (label == stringReceiverOffset)
      {
         receiverOffset = asInt(line.substr(0,6));
         valid |= validReceiverOffset;
      }

      else if (label == stringSystemDCBSapplied)
      {
         Rinex3CorrInfo tempInfo;
         tempInfo.satSys = strip(line.substr( 0, 1));
         tempInfo.name   = strip(line.substr( 2,17));
         tempInfo.source = strip(line.substr(20,40));
         infoDCBS.push_back(tempInfo);
         valid |= validSystemDCBSapplied;
      }
      else if (label == stringSystemPCVSapplied)
      {
         Rinex3CorrInfo tempInfo;
         tempInfo.satSys = strip(line.substr( 0, 1));
         tempInfo.name   = strip(line.substr( 2,17));
         tempInfo.source = strip(line.substr(20,40));
         infoPCVS.push_back(tempInfo);
         valid |= validSystemPCVSapplied;
      }
      else if (label == stringSystemScaleFac)
      {
         static const int maxObsPerLine = 12;

         satSysTemp = strip(line.substr(0,1));
         factor     = asInt(line.substr(2,4));
         numObs     = asInt(line.substr(8,2));

         int startPosition = 0;

         if ( satSysTemp == "" )
         {           // it's a continuation line; use prev. info., end pt. to start
            satSysTemp = satSysPrev;
            factor     = factorPrev;
            numObs     = numObsPrev;

            startPosition = sysSfacMap[satSysTemp].size();
         }

         // 0/blank numObs means factor applies to all obs types
         // in appropriate obsTypeList
         if (numObs == 0) numObs = mapObsTypes[satSysTemp].size();

         sfacMap tempSfacMap = sysSfacMap[satSysTemp];
         for (int i = startPosition;
                        (i < numObs) && ((i % maxObsPerLine) < maxObsPerLine); i++)
         {
            int position = 4*(i % maxObsPerLine) + 10 + 1;
            ObsID tempType(satSysTemp+strip(line.substr(position,3)));
            tempSfacMap.insert(make_pair(tempType,factor));
         }
         sysSfacMap[satSysTemp] = tempSfacMap;

         sfacMap::const_iterator iter;
         sfacMap tempmap;
         tempmap = sysSfacMap[satSysTemp];

         // save values in case next line is a continuation line
         satSysPrev = satSysTemp;
         factorPrev = factor;
         numObsPrev = numObs;

         valid |= validSystemScaleFac;
      }
      else if (label == stringSystemPhaseShift) ///< "SYS / PHASE SHIFTS"    R3.01
      {
         //map<ObsID, map<RinexSatID,double> > sysPhaseShift;
         RinexSatID sat;
         // system
         satSysTemp = strip(line.substr(0,1));

         if(satSysTemp.empty()) {                  // continuation line
            satSysTemp = satSysPrev;

            if(sysPhaseShift.find(sysPhaseShiftObsID) == sysPhaseShift.end())
            {
               FFStreamError e("SYS / PHASE SHIFT: unexpected continuation line");
               GPSTK_THROW(e);
            }

            map<RinexSatID,double>& satcorrmap(sysPhaseShift[sysPhaseShiftObsID]);
            double corr(sysPhaseShift[sysPhaseShiftObsID].begin()->second);
            for(int i=0; i<10; i++) {
               string str = strip(line.substr(19+4*i,3));
               if(str.empty()) break;
               sat = RinexSatID(str);
               satcorrmap.insert(make_pair(sat,corr));
            }
         }
         else {                                    // not a cont. line
            sat.fromString(satSysTemp);

            // obs id
            string str = strip(line.substr(2,3));

            // obsid and correction may be blank <=> unknown: ignore this
            if(!str.empty()) {
               ObsID obsid(satSysTemp+str);
               double corr(asDouble(strip(line.substr(6,8))));
               int nsat(asInt(strip(line.substr(16,2))));
               if(nsat > 0) {          // list of sats
                  map<RinexSatID,double> satcorrmap;
                  for(int i=0; i<(nsat < 10 ? nsat : 10); i++) {
                     sat = RinexSatID(strip(line.substr(19+4*i,3)));
                     satcorrmap.insert(make_pair(sat,corr));
                  }
                  sysPhaseShift.insert(make_pair(obsid,satcorrmap));
                  if(nsat > 10)        // expect continuation
                     sysPhaseShiftObsID = obsid;
               }
               else {                  // no sat, just system
                  map<RinexSatID,double> satcorrmap;
                  satcorrmap.insert(make_pair(sat,corr));
                  sysPhaseShift.insert(make_pair(obsid,satcorrmap));
               }

            }

            // save for continuation lines
            satSysPrev = satSysTemp;

            valid |= validSystemPhaseShift;
         }
      }
      else if (label == stringGlonassSlotFreqNo)
      {
         //map<RinexSatID,int> GlonassFreqNo;
         int nsat, i;
         RinexSatID sat;
         string str(strip(line.substr(0,3)));
         nsat = asInt(str);         // not used!
         for(i=0; i<8; i++) {
            str = strip(line.substr(4+i*7,3));
            if(str.empty()) break;
            sat = RinexSatID(str);
            str = strip(line.substr(8+i*7,2));
            GlonassFreqNo.insert(make_pair(sat,asInt(str)));
         }

         valid |= validGlonassFreqNo;
      }
      else if (label == stringLeapSeconds)
      {
         leapSeconds = asInt(line.substr(0,6));
         valid |= validLeapSeconds;
      }
      else if (label == stringNumSats)
      {
         numSVs = asInt(line.substr(0,6)) ;
         valid |= validNumSats;
      }
      else if (label == stringPrnObs)
      {
         static const int maxObsPerLine = 9;

         RinexSatID PRN;
         string prn, GNSS;
         int otsize;
         vector<int> numObsList;

         prn = strip(line.substr(3,3));

         if ( prn == "" ) // this is a continuation line; use last PRN
         {
            PRN = lastPRN;
            GNSS = PRN.systemChar();

            numObsList = numObsForSat[PRN]; // grab the existing list

            otsize = mapObsTypes.find(GNSS)->second.size();
            if ( otsize > numObsList.size() )    // there aren't too many entries, so ok
            {
               for (int i = numObsList.size();
                    (i < otsize) && ((i % maxObsPerLine) < maxObsPerLine); i++)
                  numObsList.push_back(asInt(line.substr(6*(i % maxObsPerLine) + 6, 6)));

               numObsForSat[PRN] = numObsList;
            }
         }
         else             // this is a new PRN line
         {
            PRN = RinexSatID(prn);
            GNSS = PRN.systemChar();

            otsize = mapObsTypes.find(GNSS)->second.size();
            if ( otsize > 0 )    // if no [SYS / # / OBS TYPE] for this GNSS, this entry is bogus
            {
               for (int i = 0; (i < otsize) && (i < maxObsPerLine); i++)
                  numObsList.push_back(asInt(line.substr(6*i + 6, 6)));

               numObsForSat[PRN] = numObsList;
            }

            lastPRN = PRN;
         }

         valid |= validPrnObs;
      }
      else if (label == stringEoH)
      {
         valid |= validEoH;
      }
      else
      {
         FFStreamError e("Unidentified label: " + label);
         GPSTK_THROW(e);
      }
   } // end of ParseHeaderRecord


   // This function parses the entire header from the given stream
   void Rinex3ObsHeader::reallyGetRecord(FFStream& ffs)
      throw(exception, FFStreamError, 
            gpstk::StringUtils::StringException)
   {
      Rinex3ObsStream& strm = dynamic_cast<Rinex3ObsStream&>(ffs);

      // If already read, just return.
      if (strm.headerRead == true) return;

      // Since we're reading a new header, we need to reinitialize all our list structures.
      // All the other objects should be ok.  This also applies if we threw an exception
      // the first time we read the header and are now re-reading it.  Some of these could
      // be full and we need to empty them.
      clear();

      string line;

      while (!(valid & validEoH))
      {
         strm.formattedGetLine(line);
         StringUtils::stripTrailing(line);

         if (line.length() == 0)
         {
            FFStreamError e("No data read");
            GPSTK_THROW(e);
         }
         else if (line.length() < 60 || line.length() > 80)
         {
            FFStreamError e("Invalid line length");
            GPSTK_THROW(e);
         }

         try
         {
            ParseHeaderRecord(line);
         }
         catch(FFStreamError& e)
         {
            GPSTK_RETHROW(e);
         }

      } // end while(not end of header)

      unsigned long allValid;
      if      (version == 3.0)  allValid = allValid30;
      else if (version == 3.01) allValid = allValid301;
      else
      {
         FFStreamError e("Unknown or unsupported RINEX version " + 
                         asString(version));
         GPSTK_THROW(e);
      }

      if ( (valid & allValid) != allValid)
      {
         FFStreamError e("Incomplete or invalid header");
         GPSTK_THROW(e);               
      }

      // If we get here, we should have reached the end of header line.
      strm.header = *this;
      strm.headerRead = true;

   } // end reallyGetRecord


   CivilTime Rinex3ObsHeader::parseTime(const string& line) const
   {
      int year, month, day, hour, min;
      double sec;
      string tsys;
      TimeSystem ts;
   
      year  = asInt(   line.substr(0,   6));
      month = asInt(   line.substr(6,   6));
      day   = asInt(   line.substr(12,  6));
      hour  = asInt(   line.substr(18,  6));
      min   = asInt(   line.substr(24,  6));
      sec   = asDouble(line.substr(30, 13));
      tsys  =          line.substr(48,  3) ;

      ts.fromString(tsys);

      return CivilTime(year, month, day, hour, min, sec, ts);
   } // end parseTime


   string Rinex3ObsHeader::writeTime(const CivilTime& civtime) const
   {
      string line;

      line  = rightJustify(asString<short>(civtime.year    )   ,  6);
      line += rightJustify(asString<short>(civtime.month   )   ,  6);
      line += rightJustify(asString<short>(civtime.day     )   ,  6);
      line += rightJustify(asString<short>(civtime.hour    )   ,  6);
      line += rightJustify(asString<short>(civtime.minute  )   ,  6);
      line += rightJustify(asString(       civtime.second,7)   , 13);
      line += rightJustify((civtime.getTimeSystem()).asString(),  8);

      return line;
   } // end writeTime


   void Rinex3ObsHeader::dump(ostream& s) const
   {
      int i,j;

      string str;
      str = system.systemChar();
      str += " (" + system.systemString() + ")";

      s << "---------------------------------- REQUIRED "
        << "----------------------------------" << endl;
      s << "Rinex Version " << fixed << setw(5) << setprecision(2) << version
        << ",  File type " << fileType << ",  System " << str << "." << endl;
      s << "Prgm: " << fileProgram << ",  Run: " << date
         << ",  By: " << fileAgency << endl;
      s << "Marker name: " << markerName << ", ";
      s << "Marker type: " << markerType << "." << endl;
      s << "Observer : " << observer << ",  Agency: " << agency << endl;
      s << "Rec#: " << recNo << ",  Type: " << recType
         << ",  Vers: " << recVers << endl;
      s << "Antenna # : " << antNo << ",  Type : " << antType << endl;
      s << "Position      (XYZ,m) : " << setprecision(4) << antennaPosition
         << "." << endl;
      s << "Antenna Delta (HEN,m) : " << setprecision(4) << antennaDeltaHEN
         << "." << endl;
      map<string,vector<ObsID> >::const_iterator iter;
      for (iter = mapObsTypes.begin(); iter != mapObsTypes.end(); iter++)
      {
         RinexSatID rsid;
         rsid.fromString(iter->first);
         s << rsid.systemString() << " Observation types ("
            << iter->second.size() << "):" << endl;
         for (i = 0; i < iter->second.size(); i++) 
            s << " Type #" << setw(2) << setfill('0') << i+1 << setfill(' ')
            << " (" << asRinex3ID(iter->second[i]) << ") "
              << asString(iter->second[i]) << endl;
      }
      s << "Time of first obs "
         << printTime(firstObs,"%04Y/%02m/%02d %02H:%02M:%06.3f %P") << endl;

      unsigned long allValid;
      if      (version == 3.0)   allValid = allValid30;
      else if (version == 3.01)  allValid = allValid301;

      s << "(This header is ";
      if ((valid & allValid) == allValid)
         s << "VALID)" << endl;
      else {
         s << "NOT VALID";
         s << " RINEX " << setprecision(2) << version << ")" << endl;
         s << "valid    = " << hex << valid << endl;
         s << "allValid = " << hex << allValid30 << endl;
         s << "v & aV   = " << hex << (valid & allValid30) << endl;
         s << dec;

         if (!(valid & validVersion        ))
            s << " Version / Type      is NOT valid" << endl;
         if (!(valid & validRunBy          ))
            s << " Pgm / Run By / Date is NOT valid" << endl;
         if (!(valid & validMarkerName     ))
            s << " Marker Name         is NOT valid" << endl;
         if (!(valid & validObserver       ))
            s << " Observer / Agency   is NOT valid" << endl;
         if (!(valid & validReceiver       ))
            s << " Receiver # / Type   is NOT valid" << endl;
         if (!(valid & validAntennaType    ))
            s << " Antenna Type        is NOT valid" << endl;
         if (!(valid & validAntennaPosition))
            s << " Antenna Position    is NOT valid" << endl;
         if (!(valid & validAntennaDeltaHEN))
            s << " Antenna Delta HEN   is NOT valid" << endl;
         if (!(valid & validSystemObsType  ))
            s << " Sys / # / Obs Type  is NOT valid" << endl;
         if (!(valid & validSystemPhaseShift))
            s << " Sys / Phase Shifts is NOT valid" << endl;
         if (!(valid & validFirstTime      ))
            s << " Time of First Obs   is NOT valid" << endl;
         if (!(valid & validEoH            ))
            s << " End of Header       is NOT valid" << endl;
      }

      s << "---------------------------------- OPTIONAL "
        << "----------------------------------" << endl;
      if (valid & validMarkerNumber     )
         s << "Marker number : " << markerNumber << endl;
      if (valid & validMarkerType       )
         s << "Marker Type : " << markerType << endl;
      if (valid & validAntennaDeltaXYZ  )
         s << "Antenna Delta    (XYZ,m) : "
           << setprecision(4) << antennaDeltaXYZ   << endl;
      if (valid & validAntennaPhaseCtr  )
         s << "Antenna PhaseCtr (XYZ,m) : "
           << setprecision(4) << antennaPhaseCtr   << endl;
      if (valid & validAntennaBsightXYZ )
         s << "Antenna B.sight  (XYZ,m) : "
           << setprecision(4) << antennaBsightXYZ  << endl;
      if (valid & validAntennaZeroDirAzi)
         s << "Antenna ZeroDir  (deg)   : "
           << setprecision(4) << antennaZeroDirAzi << endl;
      if (valid & validAntennaZeroDirXYZ)
         s << "Antenna ZeroDir  (XYZ,m) : "
           << setprecision(4) << antennaZeroDirXYZ << endl;
      if (valid & validCenterOfMass     )
         s << "Center of Mass   (XYZ,m) : "
           << setprecision(4) << antennaPhaseCtr   << endl;
      if (valid & validSigStrengthUnit  )
         s << "Signal Strenth Unit = " << sigStrengthUnit << endl;
      if (valid & validInterval         )
         s << "Interval = "
           << fixed << setw(7) << setprecision(3) << interval << endl;
      if (valid & validLastTime         )
         s << "Time of Last Obs "
           << printTime(lastObs,"%04Y/%02m/%02d %02H:%02M:%06.3f %P") << endl;
      if (valid & validReceiverOffset   )
         s << "Clock offset record is present and offsets "
           << (receiverOffset?"ARE":"are NOT") << " applied." << endl;
      if (valid & validSystemDCBSapplied)
      {
         for (i = 0; i < infoDCBS.size(); i++)
         {
            RinexSatID rsid;
            rsid.fromString(infoDCBS[i].satSys);
            s << "System DCBS Correction Applied to " << rsid.systemString()
              << " data using program " << infoDCBS[i].name << endl;
            s << " from source " << infoDCBS[i].source << "." << endl;
         }
      }
      if (valid & validSystemPCVSapplied)
      {
         for (i = 0; i < infoPCVS.size(); i++)
         {
            RinexSatID rsid;
            rsid.fromString(infoPCVS[i].satSys);
            s << "System PCVS Correction Applied to " << rsid.systemString()
              << " data using program " << infoPCVS[i].name << endl;
            s << " from source " << infoPCVS[i].source << "." << endl;
         }
      }
      if (valid & validSystemScaleFac   )
      {
         map<string, sfacMap>::const_iterator mapIter;
         // loop over GNSSes
         for (mapIter = sysSfacMap.begin(); mapIter != sysSfacMap.end(); mapIter++)
         {
            RinexSatID rsid;
            rsid.fromString(mapIter->first);
            s << rsid.systemString() << " scale factors applied:" << endl;
            map<ObsID,int>::const_iterator iter;
            // loop over scale factor map
            for(iter = mapIter->second.begin(); iter != mapIter->second.end(); iter++)
               s << "   " << iter->first.asRinex3ID() << " " << iter->second << endl;
         }
      }
      if (valid & validSystemPhaseShift )
      {
         map<ObsID, map<RinexSatID,double> >::const_iterator it;
         for(it=sysPhaseShift.begin(); it!=sysPhaseShift.end(); ++it) {
            map<RinexSatID,double>::const_iterator jt;
            for(jt=it->second.begin(); jt!=it->second.end(); ++jt)
               s << "Phase shift correction "
                  << fixed << setprecision(5)
                  << setw(8) << jt->second << " cycles applied to "
                  << jt->first.toString() << " " << it->first << endl;
         }
      }
      if (valid & validGlonassFreqNo     )
      {
         int n(0);
         map<RinexSatID,int>::const_iterator it;
         s << "GLONASS frequency channels:\n";
         for(it=GlonassFreqNo.begin(); it!=GlonassFreqNo.end(); ++it) {
            s << " " << it->first.toString() << " " << setw(2) << it->second;
            if(++n > 1 && (n%8)==0) s << endl;
         }
         if((n%8) != 0) s << endl;
      }
      if (valid & validLeapSeconds      )
         s << "Leap seconds: " << leapSeconds << endl;
      if (valid & validNumSats          )
         s << "Number of Satellites with data : " << numSVs << endl;
      if (valid & validPrnObs           )
      {
         s << " PRN and number of observations for each obs type:" << endl;
         map<RinexSatID, vector<int> >::const_iterator sat_itr = numObsForSat.begin();
         while (sat_itr != numObsForSat.end())
         {
            vector<int> obsvec = sat_itr->second;
            s << " " << sat_itr->first.toString() << " ";
            for (i = 0; i < obsvec.size(); i++)
              s << " " << setw(6) << obsvec[i];
            s << endl;
            sat_itr++;
         }
      }
      if (commentList.size() && !(valid & validComment))
         s << " Comment is NOT valid" << endl;
      s << "Comments (" << commentList.size() << ") :" << endl;
      for (i = 0; i < commentList.size(); i++)
         s << commentList[i] << endl;

      s << "-------------------------------- END OF HEADER "
        << "--------------------------------" << endl;
   } // end dump


} // namespace gpstk