#include <iostream>
#include <vector>
#include <regex>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <cstring>
#include <cctype>
using namespace std;

const char* crt = "\n-------------------------------------------\n";
static mutex coutMtx; // za uredan ispis iz vise thread-ova

enum Zanr {
	ROK, POP, JAZZ, ELEKTRONSKA, HIP_HOP
};

int reverseNum(int num) {
	int reverseNum = 0;
	while (num > 0)
	{
		reverseNum = reverseNum * 10 + (num % 10);
		num /= 10;
	}
	return reverseNum;
}
string GenerisiID(const char* imePrezime, int broj) {
	string ID;

	ID += toupper(imePrezime[0]);
	if (broj < 10) ID += "000";
	else if (broj < 100) ID += "00";
	else if (broj < 1000) ID += "0";
	const char* prezime = strchr(imePrezime, ' ');
	if (prezime != nullptr && strlen(prezime) > 1) ID += toupper(prezime[1]);
	int obrnutiBroj = reverseNum(broj);
	ID += to_string(obrnutiBroj);

	return ID;
}
bool ValidirajID(const string& ID) {
	return regex_match(ID, regex("([A-Z]000[A-Z]\\d|[A-Z]00[A-Z]\\d{2}|[A-Z]0[A-Z]\\d{3}|[A-Z]{2}\\d{4})"));
}

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr) return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[] sadrzaj;
	return temp;
}

template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int* _trenutno;
public:
	Kolekcija() { _trenutno = new int(0); }
	Kolekcija(const Kolekcija& obj) {
		_trenutno = new int(obj.getTrenutno());
		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = obj.getElement1(i);
			_elementi2[i] = obj.getElement2(i);
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj)
		{
			delete _trenutno; _trenutno = nullptr;

			_trenutno = new int(obj.getTrenutno());
			for (int i = 0; i < *_trenutno; i++)
			{
				_elementi1[i] = obj.getElement1(i);
				_elementi2[i] = obj.getElement2(i);
			}
		}
		return *this;
	}
	~Kolekcija() {
		delete _trenutno; _trenutno = nullptr;
	}
	T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
	const T1& getElement1(int lokacija) const { return _elementi1[lokacija]; }
	const T2& getElement2(int lokacija) const { return _elementi2[lokacija]; }
	int getTrenutno() const { return *_trenutno; }
	friend ostream& operator<<(ostream& COUT, Kolekcija& obj) {
		for (int i = 0; i < obj.getTrenutno(); i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	void AddElement(const T1& el1, const T2& el2) {
		_elementi1[*_trenutno] = el1;
		_elementi2[*_trenutno] = el2;

		(*_trenutno)++;
	}
	Kolekcija InsertAt(const T1& el1, const T2& el2, int lokacija) {
		if (*_trenutno >= max) return *this;
		if (lokacija < 0 || lokacija > *_trenutno) return *this;

		for (int i = *_trenutno; i > lokacija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}

		_elementi1[lokacija] = el1;
		_elementi2[lokacija] = el2;

		(*_trenutno)++;
		return *this;
	}
	Kolekcija RemoveRange(int odLokacije, int doLokacije) {
		if (odLokacije < 0 || doLokacije < odLokacije || doLokacije >= *_trenutno)
			throw exception("Opseg nije validan");
		Kolekcija temp;

		for (int i = odLokacije; i <= doLokacije; i++)
		{
			temp.AddElement(_elementi1[i], _elementi2[i]);
		}
		int brojElemenata = doLokacije - odLokacije + 1;
		for (int i = odLokacije + brojElemenata; i < *_trenutno; i++)
		{
			_elementi1[i - brojElemenata] = _elementi1[i];
			_elementi2[i - brojElemenata] = _elementi2[i];
		}

		*_trenutno -= brojElemenata;
		return temp;
	}
};

class Datum {
	int* _sat, * _minuta, * _sekunda;
public:
	Datum(int sat = 20, int minuta = 0, int sekunda = 0) {
		_sat = new int(sat);
		_minuta = new int(minuta);
		_sekunda = new int(sekunda);
	}
	Datum(const Datum& obj) {
		_sat = new int(*obj._sat);
		_minuta = new int(*obj._minuta);
		_sekunda = new int(*obj._sekunda);
	}
	Datum& operator=(const Datum& obj) {
		if (this != &obj)
		{
			delete _sat; _sat = nullptr;
			delete _minuta; _minuta = nullptr;
			delete _sekunda; _sekunda = nullptr;

			_sat = new int(*obj._sat);
			_minuta = new int(*obj._minuta);
			_sekunda = new int(*obj._sekunda);
		}
		return *this;
	}
	~Datum() {
		delete _sat; _sat = nullptr;
		delete _minuta; _minuta = nullptr;
		delete _sekunda; _sekunda = nullptr;
	}
	friend ostream& operator<<(ostream& COUT, const Datum& obj) {
		COUT << *obj._sat << ":" << *obj._minuta << ":" << *obj._sekunda;
		return COUT;
	}

	int getSati()const { return *_sat; };
	int getMinuta()const { return *_minuta; };
	int getSekunda()const { return *_sekunda; };

	int ToSekunde() {
		return *_sat * 3600 + *_minuta * 60 + *_sekunda;
	}
};

class Nagrada {
	Datum* _vrijemeUrucenja;
	char* _napomena;
public:
	Nagrada(Datum vrijeme, const char* napomena) {
		_napomena = GetNizKaraktera(napomena);
		_vrijemeUrucenja = new Datum(vrijeme);
	}
	Nagrada(const Nagrada& obj) {
		_vrijemeUrucenja = new Datum(*obj._vrijemeUrucenja);
		_napomena = GetNizKaraktera(obj._napomena);
	}
	Nagrada& operator=(const Nagrada& obj) {
		if (this != &obj)
		{
			delete[] _napomena; _napomena = nullptr;
			delete _vrijemeUrucenja; _vrijemeUrucenja = nullptr;

			_vrijemeUrucenja = new Datum(*obj._vrijemeUrucenja);
			_napomena = GetNizKaraktera(obj._napomena);
		}
		return *this;
	}
	~Nagrada() {
		delete[] _napomena; _napomena = nullptr;
		delete _vrijemeUrucenja; _vrijemeUrucenja = nullptr;
	}
	Datum GetVrijemeUrucenja() { return *_vrijemeUrucenja; }
	Datum GetVrijemeUrucenja() const { return *_vrijemeUrucenja; }
	char* GetNapomena() { return _napomena; }
	const char* GetNapomena() const { return _napomena; }
	friend ostream& operator<<(ostream& COUT, const Nagrada& obj) {
		COUT << *obj._vrijemeUrucenja << " -> " << obj._napomena;
		return COUT;
	}
	bool operator==(const Nagrada& obj) {
		return strcmp(_napomena, obj._napomena) == 0;
	}
};

class Izvodjac {
	static int _id;
	char* _ID;
	char* _imePrezime;
	vector<Nagrada*> _nagrade;
public:
	Izvodjac(const char* imePrezime) {
		_imePrezime = GetNizKaraktera(imePrezime);
		_ID = GetNizKaraktera(GenerisiID(_imePrezime, _id++).c_str());
	}
	Izvodjac(const Izvodjac& obj) {
		_imePrezime = GetNizKaraktera(obj.GetImePrezime());
		_ID = GetNizKaraktera(obj.GetID());
		for (size_t i = 0; i < obj._nagrade.size(); i++)
		{
			_nagrade.push_back(new Nagrada(*obj._nagrade[i]));
		}
	}
	Izvodjac& operator=(const Izvodjac& obj) {
		if (this != &obj)
		{
			delete[] _ID; _ID = nullptr;
			delete[] _imePrezime; _imePrezime = nullptr;
			for (size_t i = 0; i < _nagrade.size(); i++) {
				delete _nagrade[i];
				_nagrade[i] = nullptr;
			}
			_nagrade.clear();

			_imePrezime = GetNizKaraktera(obj.GetImePrezime());
			_ID = GetNizKaraktera(obj.GetID());
			for (size_t i = 0; i < obj._nagrade.size(); i++)
			{
				_nagrade.push_back(new Nagrada(*obj._nagrade[i]));
			}
		}
		return *this;
	}
	~Izvodjac() {
		delete[] _ID; _ID = nullptr;
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _nagrade.size(); i++) {
			delete _nagrade[i];
			_nagrade[i] = nullptr;
		}
		_nagrade.clear();
	}
	char* GetImePrezime() { return _imePrezime; }
	const char* GetImePrezime() const { return _imePrezime; }
	char* GetID() { return _ID; }
	const char* GetID() const { return _ID; }
	vector<Nagrada*>& GetNagrade() { return _nagrade; }
	const vector<Nagrada*>& GetNagrade() const { return _nagrade; }
	friend ostream& operator<<(ostream& COUT, Izvodjac& obj) {
		COUT << obj._ID << " -> " << obj._imePrezime << endl;
		for (size_t i = 0; i < obj._nagrade.size(); i++)
			COUT << *obj._nagrade[i] << endl;
		return COUT;
	}
	bool operator==(const Izvodjac& obj) {
		return strcmp(_ID, obj._ID) == 0;
	}
};
int Izvodjac::_id = 1;

class Bend {
	Zanr _zanr;
	vector<Izvodjac> _clanovi;
public:
	Bend(Zanr zanr = ROK) {
		_zanr = zanr;
	}
	Bend(const Bend& obj) {
		_zanr = obj._zanr;
		_clanovi = obj._clanovi;
	}
	Bend& operator=(const Bend& obj) {
		if (this != &obj)
		{
			_zanr = obj._zanr;
			_clanovi = obj._clanovi;
		}
		return *this;
	}
	~Bend() {};
	Zanr GetZanr() { return _zanr; }
	Zanr GetZanr() const { return _zanr; }
	vector<Izvodjac>& GetClanovi() { return _clanovi; }
	const vector<Izvodjac>& GetClanovi() const { return _clanovi; }
	void AddClana(const Izvodjac& clan) {
		for (auto& c : _clanovi) {
			if (c == clan) throw exception("Clan vec postoji");
		}
		_clanovi.push_back(clan);
	}
};

class Festival {
	Kolekcija<Bend*, Bend*, 20> _nastupi;

	// Pomocna funkcija - salje "mailove" svim clanovima proslijedjenog benda.
	// Prima kopije/pointere koji su nezavisni od stack frame-a AddNagrada, pa je
	// bezbjedno pozivati je iz detach-ovanog thread-a.
	static void PosaljiMailoveZaBend(Bend* bend, Nagrada nagrada, string imePrezimeDobitnika) {
		for (size_t h = 0; h < bend->GetClanovi().size(); h++)
		{
			Izvodjac& clan = bend->GetClanovi()[h];

			lock_guard<mutex> lg(coutMtx);
			cout << "To: " << clan.GetID() << "@festival2026.com" << endl;
			cout << "From: info@festival2026.com" << endl;
			cout << "Subject: Informacija" << endl;
			cout << "Postovani," << endl;
			cout << "U " << nagrada.GetVrijemeUrucenja()
				<< " sati, izvodjac " << imePrezimeDobitnika
				<< " je osvojio nagradu na ovom nastupu." << endl;
			cout << "Trenutni broj nagrada benda je: "
				<< bend->GetClanovi().size()
				<< endl;
			cout << "Neka pobijedi bolja izvedba." << endl;

			this_thread::sleep_for(chrono::seconds(2));
		}
	}

public:
	Festival() {};
	Festival(const Festival& obj) { _nastupi = obj._nastupi; }
	Festival& operator=(const Festival& obj) {
		if (this != &obj)
		{
			_nastupi = obj._nastupi;
		}
		return *this;
	}
	~Festival() {};
	Kolekcija<Bend*, Bend*, 20>& GetNastupi() { return _nastupi; }

	// AddNastup onemogucava ponovni "duel" dva ista benda tokom istog festivala - baca izuzetak
	void AddNastup(Bend& b1, Bend& b2) {
		for (int i = 0; i < _nastupi.getTrenutno(); i++)
		{
			if ((_nastupi.getElement1(i)->GetZanr() == b1.GetZanr() && _nastupi.getElement2(i)->GetZanr() == b2.GetZanr()) ||
				(_nastupi.getElement1(i)->GetZanr() == b2.GetZanr() && _nastupi.getElement2(i)->GetZanr() == b1.GetZanr())) {
				throw exception("Dva benda se vec susrecu");
			}
		}
		_nastupi.AddElement(new Bend(b1), new Bend(b2));
	}

	// AddNagrada dodaje nagradu izvodjacu po ID-u ili imenu i prezimenu, unutar konkretnog nastupa
	// dva benda. Onemoguciti dodavanje potpuno iste nagrade (isti izvodjac, isto vrijeme, ista napomena).
	// Nakon uspjesno dodane nagrade, u posebnom thread-u, svim clanovima OBA benda (na email
	// IZVEDEN_IZ_ID@festival2026.com) poslati mail u razmaku od 2 sekunde.
	bool AddNagrada(Zanr zanrBenda1, Zanr zanrBenda2, const char* idIliImePrezime, Nagrada nagrada) {
		for (int i = 0; i < _nastupi.getTrenutno(); i++)
		{
			Bend* bend1 = _nastupi.getElement1(i);
			Bend* bend2 = _nastupi.getElement2(i);

			bool poklapanjeZanrova =
				(bend1->GetZanr() == zanrBenda1 && bend2->GetZanr() == zanrBenda2) ||
				(bend1->GetZanr() == zanrBenda2 && bend2->GetZanr() == zanrBenda1);

			if (!poklapanjeZanrova)
				continue;

			// Trazimo izvodjaca prvo u bend1, pa u bend2
			Bend* bendovi[2] = { bend1, bend2 };

			for (int b = 0; b < 2; b++)
			{
				for (size_t j = 0; j < bendovi[b]->GetClanovi().size(); j++)
				{
					Izvodjac& izvodjac = bendovi[b]->GetClanovi()[j];

					if (strcmp(izvodjac.GetImePrezime(), idIliImePrezime) == 0 ||
						strcmp(izvodjac.GetID(), idIliImePrezime) == 0)
					{
						// provjera duplikata (isti izvodjac, isto vrijeme, ista napomena)
						for (size_t k = 0; k < izvodjac.GetNagrade().size(); k++)
						{
							if (strcmp(izvodjac.GetNagrade()[k]->GetNapomena(), nagrada.GetNapomena()) == 0 &&
								izvodjac.GetNagrade()[k]->GetVrijemeUrucenja().ToSekunde() ==
								nagrada.GetVrijemeUrucenja().ToSekunde())
							{
								return false;
							}
						}

						izvodjac.GetNagrade().push_back(new Nagrada(nagrada));

						// Sve sto thread-u treba kopiramo/uzimamo kao stabilne pointere
						// PRIJE pokretanja thread-a, da izbjegnemo use-after-free nakon
						// sto se AddNagrada vrati (t.detach()).
						string imeDobitnika = izvodjac.GetImePrezime();
						Nagrada nagradaKopija = nagrada; // eksplicitna kopija (copy ctor)

						thread t([bend1, bend2, nagradaKopija, imeDobitnika]() {
							PosaljiMailoveZaBend(bend1, nagradaKopija, imeDobitnika);
							PosaljiMailoveZaBend(bend2, nagradaKopija, imeDobitnika);
							});
						t.detach();

						return true;
					}
				}
			}
		}

		return false;
	}

	friend ostream& operator<<(ostream& COUT, Festival& obj) {
		for (int i = 0; i < obj._nastupi.getTrenutno(); i++)
		{
			Bend* b1 = obj._nastupi.getElement1(i);
			Bend* b2 = obj._nastupi.getElement2(i);

			COUT << crt;
			COUT << "(" << b1->GetClanovi().size() << ") ";
			switch (b1->GetZanr())
			{
			case ROK: COUT << "ROK"; break;
			case POP: COUT << "POP"; break;
			case JAZZ: COUT << "JAZZ"; break;
			case ELEKTRONSKA: COUT << "ELEKTRONSKA"; break;
			case HIP_HOP: COUT << "HIP_HOP"; break;
			}
			COUT << " : ";
			switch (b2->GetZanr())
			{
			case ROK: COUT << "ROK"; break;
			case POP: COUT << "POP"; break;
			case JAZZ: COUT << "JAZZ"; break;
			case ELEKTRONSKA: COUT << "ELEKTRONSKA"; break;
			case HIP_HOP: COUT << "HIP_HOP"; break;
			}
			COUT << " (" << b2->GetClanovi().size() << ")";
			COUT << crt;

			size_t maxClanova = max(b1->GetClanovi().size(), b2->GetClanovi().size());
			for (size_t k = 0; k < maxClanova; k++)
			{
				if (k < b1->GetClanovi().size())
					COUT << b1->GetClanovi()[k].GetImePrezime();
				COUT << "\t\t";
				if (k < b2->GetClanovi().size())
					COUT << b2->GetClanovi()[k].GetImePrezime();
				COUT << endl;
			}
			COUT << crt;
		}
		return COUT;
	}

	// operator() vraca sve izvodjace na festivalu koji imaju broj nagrada >= proslijedjenoj vrijednosti,
	// sortirane opadajuce po broju nagrada (iskoristiti lambda izraz za sortiranje)
	vector<Izvodjac*> operator()(int minBrojNagrada) {
		vector<Izvodjac*> rezultat;

		for (int i = 0; i < _nastupi.getTrenutno(); i++)
		{
			Bend* bendovi[2] = { _nastupi.getElement1(i), _nastupi.getElement2(i) };

			for (int b = 0; b < 2; b++)
			{
				for (auto& izvodjac : bendovi[b]->GetClanovi())
				{
					if ((int)izvodjac.GetNagrade().size() >= minBrojNagrada)
						rezultat.push_back(&izvodjac);
				}
			}
		}

		sort(rezultat.begin(), rezultat.end(), [](Izvodjac* a, Izvodjac* b) {
			return a->GetNagrade().size() > b->GetNagrade().size();
			});

		return rezultat;
	}
};

int main() {
	/*
	Globalna funkcija GenerisiID vraca ID izvodjaca na osnovu vrijednosti
	proslijedjenih parametara.
	*/
	cout << GenerisiID("Dino Merlin", 7) << endl;
	cout << GenerisiID("Hari Varesanovic", 25) << endl;
	cout << GenerisiID("Amira Medunjanin", 312) << endl;
	cout << GenerisiID("edo maajka", 4089) << endl;

	if (ValidirajID("D000M7"))
		cout << "ID VALIDAN" << endl;
	if (ValidirajID("H00V52"))
		cout << "ID VALIDAN" << endl;
	if (!ValidirajID("A00M213"))
		cout << "ID NIJE VALIDAN" << endl;
	if (!ValidirajID("em9804"))
		cout << "ID NIJE VALIDAN" << endl;

	int kolekcijaTestSize = 9;
	Kolekcija<int, int, 10> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);
	cout << kolekcija1 << crt;

	Kolekcija<int, int, 10> kolekcija2 = kolekcija1.InsertAt(10, 10, 0);
	cout << kolekcija2 << crt;

	Kolekcija<int, int, 10> kolekcija3 = kolekcija1.RemoveRange(1, 3);
	cout << kolekcija3 << endl;
	cout << kolekcija1 << crt;

	kolekcija1 = kolekcija3;
	cout << kolekcija1;

	Datum
		prvaNagrada203115(20, 31, 15),
		drugaNagrada204502(20, 45, 2),
		trecaNagrada211809(21, 18, 9),
		cetvrtaNagrada213327(21, 33, 27);

	Izvodjac dino("Dino Merlin"), hari("Hari Varesanovic"),
		amira("Amira Medunjanin"), edo("Edo Maajka");

	if (strcmp(dino.GetID(), "D000M1") == 0 && strcmp(hari.GetID(), "H000V2") == 0)
		cout << "ID se uspjesno generise!" << endl;

	Nagrada prvaN(prvaNagrada203115, "publika ustala na noge"),
		drugaN(drugaNagrada204502, "najbolji vokal vecеri"),
		trecaN(trecaNagrada211809, "najduzi aplauz na festivalu"),
		cetvrtaN(cetvrtaNagrada213327, "specijalno priznanje zirija");

	Bend rokBend(ROK), popBend(POP);
	rokBend.AddClana(dino);
	rokBend.AddClana(hari);
	popBend.AddClana(amira);
	popBend.AddClana(edo);

	try
	{
		rokBend.AddClana(dino);
	}
	catch (exception& obj)
	{
		cout << obj.what();
	}

	Festival muzickiFestival2026;
	muzickiFestival2026.AddNastup(rokBend, popBend);
	try
	{
		muzickiFestival2026.AddNastup(rokBend, popBend);
	}
	catch (exception& obj)
	{
		cout << obj.what();
	}

	if (muzickiFestival2026.AddNagrada(ROK, POP, "D000M1", prvaN))
		cout << "Nagrada uspjesno dodata" << endl;
	if (!muzickiFestival2026.AddNagrada(ROK, POP, "Dino Merlin", prvaN))
		cout << "Nagrada NIJE uspjesno dodata" << endl;
	if (muzickiFestival2026.AddNagrada(ROK, POP, "H000V2", drugaN))
		cout << "Nagrada uspjesno dodata" << endl;
	if (muzickiFestival2026.AddNagrada(ROK, POP, "Hari Varesanovic", trecaN))
		cout << "Nagrada uspjesno dodata" << endl;
	if (muzickiFestival2026.AddNagrada(ROK, POP, "Amira Medunjanin", cetvrtaN))
		cout << "Nagrada uspjesno dodata" << endl;

	// sacekaj da detach-ovani thread-ovi zavrse ispis prije glavnog ispisa
	this_thread::sleep_for(chrono::seconds(9));

	cout << muzickiFestival2026;
	/*
	-------------------------------------------
	(3) ROK : POP (1)
	-------------------------------------------
	Dino Merlin        Amira Medunjanin
	Hari Varesanovic
	Hari Varesanovic
	-------------------------------------------
	*/

	vector<Izvodjac*> izvodjaci = muzickiFestival2026(2);
	for (size_t i = 0; i < izvodjaci.size(); i++)
		cout << izvodjaci[i]->GetImePrezime() << endl;

	cin.get();
	system("pause>0");
}