#include <Serial.h>
#include <Range.h>
//
#include <stdio.h>
//
#include <map>
#include <list>
#include <algorithm>

///////////////////////////////////////////////////////////

typedef std::map<thomson::Ssid, std::list<thomson::Serial::Password> > Map;
void mapadd (Map& map, thomson::Ssid const& k, thomson::Serial::Password const& v) {
	map[k].push_back(v);
}

struct PassPrinter {
	typedef char SsidStr[thomson::SSID_LENGTH * 2 + 1];

	FILE* const	fout;
	SsidStr&	ssid;

	PassPrinter (SsidStr& ssid, FILE* const fout): ssid(ssid), fout(fout) {}

	void operator () (Map::mapped_type::value_type const& password) const {
		fprintf(fout, "%6s : %11s\n", &ssid[0], &password.pass[0]);
	}
};

struct SsidPassPrinter {
	typedef Map::value_type	Pair;

	FILE* const fout;
	SsidPassPrinter (FILE* const fout): fout(fout) {}

	void operator () (Pair const& pair) const {
		PassPrinter::SsidStr ssid;
		pair.first.GetSsid(ssid);

		std::for_each(pair.second.begin(), pair.second.end(), PassPrinter(ssid, fout));
	}
};

///////////////////////////////////////////////////////////

static int main0 (int const argc, char const* const* const argv) {
	Map ssids;
	thomson::Range<unsigned char> years(11, 12), weeks(1, 52), salt0(0, 36), salt1(0, 36), salt2(0, 36);
//	thomson::Range<unsigned char> years(9, 9), weeks(1, 5), salt0(0, 3), salt1(0, 3), salt2(0, 3);
	thomson::Serial serial;
	thomson::Ssid ssid;
	thomson::Serial::Password password;

	for (; years.HasNext(); weeks.Reset()) {
		unsigned char const year = years.Next();

		for (; weeks.HasNext(); salt0.Reset()) {
			unsigned char const week = weeks.Next();
			printf("Testing week %d/52, year %d...\n", week, 2000 + year);

			for (; salt0.HasNext(); salt1.Reset()) {
				unsigned char const s0 = salt0.Next();

				for (; salt1.HasNext(); salt2.Reset()) {
					unsigned char const s1 = salt1.Next();

					for (; salt2.HasNext(); ) {
						unsigned char const s2 = salt2.Next();

						serial.SetYear(year);
						serial.SetWeek(week);
						serial.SetSalt0(s0);
						serial.SetSalt1(s1);
						serial.SetSalt2(s2);
						
						serial.GetSsid(&ssid, &password);
						
						mapadd(ssids, ssid, password);	
					}
				}
			}
		}
	}

	{
		char const Filepath[] = "./allpass.txt";
		printf("Writing all to file %s...\n", &Filepath[0]);

		FILE* fout = NULL;
		errno_t const fopenResult = fopen_s(&fout, &Filepath[0], "w");
		if (fopenResult == 0) {
			assert(fout);

			std::for_each(ssids.begin(), ssids.end(), SsidPassPrinter(fout));

			fclose(fout);
		}
	}


	return 0;
}

int main (int argc, char** argv) {
	return main0(argc, argv);
}
