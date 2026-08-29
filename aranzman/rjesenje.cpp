#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <thread>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

string crt = "\n---------------------------------------------------------------------------------\n";

char* AlocirajTekst(const char* tekst) {
	if (tekst == nullptr) return nullptr;
	size_t velicina = strlen(tekst) + 1;
	char* noviTekst = new char[velicina];
	strcpy_s(noviTekst, velicina, tekst);
	return noviTekst;
}

enum TipAranzmana { ODMOR, EKSKURZIJA, AVANTURA, POSLOVNO };
const char* TipAranzmanaOznake[] = { "OD", "EK", "AV", "PO" };
const char* TipAranzmanaNazivi[] = { "ODMOR", "EKSKURZIJA", "AVANTURA", "POSLOVNO" };

/*
	Funkcija GenerisiOznaku generise oznaku u formatu: TA-TP-BBBB-IN
	TA -> fiksna oznaka agencije,
	TP -> oznaka tipa aranzmana: OD (odmor), EK (ekskurzija), AV (avantura), PO (poslovno),
	BBBB -> redni broj popunjen nulama na slobodnim mjestima (raspon 1-9999),
	IN -> inicijali prve i posljednje rijeci imena i prezimena.
	Za neispravne podatke funkcija vraca "TA-XX-0000-XX".
	*/
string GenerisiOznaku(const char* imePrezime, int redniBroj, TipAranzmana tip) {
	if (redniBroj < 1 || redniBroj > 9999) return "TA-XX-0000-XX";
	if (imePrezime == nullptr && strlen(imePrezime) < 1) return "TA-XX-0000-XX";
	if (tip != ODMOR && tip != EKSKURZIJA && tip != AVANTURA && tip != POSLOVNO)  return "TA-XX-0000-XX";

	string oznaka;

	oznaka += "TA-";
	switch (tip)
	{
	case ODMOR: oznaka += "OD"; break;
	case EKSKURZIJA: oznaka += "EK"; break;
	case AVANTURA: oznaka += "AV";  break;
	case POSLOVNO: oznaka += "PO"; break;
	default:  return "TA-XX-0000-XX"; break;
	}
	oznaka += "-";

	if (redniBroj < 10) oznaka += "000";
	else if (redniBroj < 100) oznaka += "00";
	else if (redniBroj < 1000) oznaka += "0";
	oznaka += to_string(redniBroj);
	oznaka += "-";

	oznaka += toupper(imePrezime[0]);
	const char* zadnjiRazmak = strrchr(imePrezime, ' ');
	if (zadnjiRazmak != nullptr && strlen(zadnjiRazmak) > 1) oznaka += toupper(zadnjiRazmak[1]);
	else return "TA-XX-0000-XX";

	return oznaka;
};
// koristeci regex, funkcija ValidirajOznaku provjerava da li je oznaka
	// zapisana u prethodno definisanom formatu.
bool ValidirajOznaku(const string& oznaka) {
	return regex_match(oznaka, regex("TA-(OD|EK|AV|PO)-(?!0000)[0-9]{4}-[A-Z]{2}"));
};

/*
Klasa Kolekcija cuva parove elemenata preko pokazivaca (T1*, T2*) u nizovima
fiksne velicine max. Kako klasa upravlja dinamicki alociranim resursima,
potrebno je implementirati i konstruktor kopije i operator dodjele (pravilo
trojke), tako da kopija i original imaju potpuno nezavisne resurse.
*/
template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max];
	T2* _elementi2[max];
	int _trenutno;
public:
	Kolekcija() : _trenutno(0) {
		for (int i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	Kolekcija(const Kolekcija& obj) {
		_trenutno = obj.GetTrenutno();
		for (int i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = new T1(obj.GetElement1(i));
			_elementi2[i] = new T2(obj.GetElement2(i));
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj)
		{
			for (int i = 0; i < _trenutno; i++) {
				delete _elementi1[i];
				delete _elementi2[i];
				_elementi1[i] = nullptr;
				_elementi2[i] = nullptr;
			}
			_trenutno = 0;

			_trenutno = obj.GetTrenutno();
			for (int i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = new T1(obj.GetElement1(i));
				_elementi2[i] = new T2(obj.GetElement2(i));
			}
		}
		return *this;
	}
	~Kolekcija() {
		for (int i = 0; i < _trenutno; i++) {
			delete _elementi1[i];
			delete _elementi2[i];
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	int GetTrenutno() const { return _trenutno; }
	T1& GetElement1(int indeks) { return *_elementi1[indeks]; }
	T2& GetElement2(int indeks) { return *_elementi2[indeks]; }
	const T1& GetElement1(int indeks) const { return *_elementi1[indeks]; }
	const T2& GetElement2(int indeks) const { return *_elementi2[indeks]; }
	T1& operator[](int indeks) { return *_elementi1[indeks]; }

	void Dodaj(const T1& el1, const T2& el2) {
		if (_trenutno >= max) return;

		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno] = new T2(el2);

		_trenutno++;
	}
	
	// DodajNaPoziciju dodaje novi par na lokaciju/indeks definisanu prvim parametrom,
	// pomjera postojece pokazivace udesno i vraca trenutno stanje kolekcije (*this).
	// U slucaju popunjene kolekcije ili neispravne lokacije potrebno je baciti izuzetak.
	Kolekcija DodajNaPoziciju(int pozicija, const T1& el1, const T2& el2) {
		if (_trenutno >= max) throw exception("Kolekcija puna");
		if (pozicija < 0 || pozicija > _trenutno) throw exception("Pozicija nije validna");

		for (size_t i = _trenutno; i > pozicija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}

		_elementi1[pozicija] = new T1(el1);
		_elementi2[pozicija] = new T2(el2);

		_trenutno++;
		return *this;
	}
	// UkloniSaPozicije uklanja par sa proslijedjene pozicije/indeksa, preostale elemente
	// pomjera ulijevo i vraca uklonjeni par u obliku pair<T1, T2>.
	// Za poziciju izvan opsega baca se izuzetak.
	pair<T1, T2> UkloniSaPozicije(int pozicija) {
		if (pozicija < 0 || pozicija >= _trenutno) throw exception("Pozicija nije validna");
		pair<T1, T2> temp;

		temp.first = *_elementi1[pozicija];
		temp.second = *_elementi2[pozicija];

		for (size_t i = pozicija; i < _trenutno - 1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}

		_trenutno--;
		return temp;
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (int i = 0; i < obj._trenutno; i++)
			COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
		return COUT;
	}
};

class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minute;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minute = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minute = new int(minute);
	}
	DatumVrijeme(const DatumVrijeme& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minute = new int(*obj._minute);
	}
	DatumVrijeme& operator=(const DatumVrijeme& obj) {
		if (this != &obj)
		{
			delete _dan; delete _mjesec; delete _godina; delete _sati; delete _minute;
			_dan = _mjesec = _godina = _sati = _minute = nullptr;

			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
			_sati = new int(*obj._sati);
			_minute = new int(*obj._minute);
		}
		return *this;
	}
	// ToString vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM, ukljucujuci pocetne nule
	string ToString() const {
		stringstream ss;

		ss << setw(2) << setfill('0') << *_dan << "."
			<< setw(2) << setfill('0') << *_mjesec << "."
			<< setw(2) << setfill('0') << *_godina << " "
			<< setw(2) << setfill('0') << *_sati << ":"
			<< setw(2) << setfill('0') << *_minute;

		return ss.str();
	}
	friend ostream& operator<<(ostream& COUT, const DatumVrijeme& obj) {
		COUT << obj.ToString();
		return COUT;
	}
	~DatumVrijeme() {
		delete _dan; delete _mjesec; delete _godina; delete _sati; delete _minute;
		_dan = _mjesec = _godina = _sati = _minute = nullptr;
	}
	const int getDan() const { return *_dan; };
	const int getMjesec() const { return *_mjesec; };
	const int getGodina() const { return *_godina; };
	const int getSati() const { return *_sati; };
	const int getMinute() const { return *_minute; };
	bool operator>(const DatumVrijeme& obj) const {
		if (*_godina > obj.getGodina()) return true;
		if (*_godina < obj.getGodina()) return false;

		if (*_mjesec > obj.getMjesec()) return true;
		if (*_mjesec < obj.getMjesec()) return false;

		if (*_dan > obj.getDan()) return true;
		if (*_dan < obj.getDan()) return false;

		if (*_sati > obj.getSati()) return true;
		if (*_sati < obj.getSati()) return false;

		if (*_minute > obj.getMinute()) return true;
		if (*_minute < obj.getMinute()) return false;
	}
	bool operator==(const DatumVrijeme& obj) const {
		return *_dan == obj.getDan() && *_mjesec == obj.getMjesec() && *_godina == obj.getGodina() && *_sati == obj.getSati() && *_minute == obj.getMinute();
	}
	bool daLiJeIstiDatum(const DatumVrijeme& noviDatum) {
		if (noviDatum.getDan() == *_dan &&
			noviDatum.getMjesec() == *_mjesec &&
			noviDatum.getGodina() == *_godina &&
			noviDatum.getSati() == *_sati &&
			noviDatum.getMinute() == *_minute) return true;
		return false;
	}
	int ukupnoDana() const  {
		int ukupnoDana = 0;

		for (int godina = 1; godina < getGodina(); godina++) {
			if (godina % 400 == 0 || (godina % 4 == 0 && godina % 100 != 0))
				ukupnoDana += 366;
			else ukupnoDana += 365;
		}

		int DaniUMjesecu[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		for (int mjesec = 1; mjesec < getMjesec(); mjesec++) {
			ukupnoDana = DaniUMjesecu[mjesec - 1];
			if (mjesec == 2 && (getGodina() % 400 == 0 || (getGodina() % 4 == 0 && getGodina() % 100 != 0)))
				ukupnoDana++;
		}

		ukupnoDana += getDan();
		return ukupnoDana;
	}
};

class Aranzman {
	char* _oznaka;
	char* _naziv;
	TipAranzmana _tip;
	DatumVrijeme _pocetak;
	int _trajanjeDana;
public:
	Aranzman(const char* naziv, const char* imePrezimePutnika, int redniBroj,
		TipAranzmana tip, DatumVrijeme pocetak, int trajanjeDana)
		: _tip(tip), _pocetak(pocetak), _trajanjeDana(trajanjeDana) {
		_naziv = AlocirajTekst(naziv);
		_oznaka = AlocirajTekst(GenerisiOznaku(imePrezimePutnika, redniBroj, tip).c_str());
	}
	Aranzman(const Aranzman& obj) {
		_oznaka = AlocirajTekst(obj.GetOznaka());
		_naziv = AlocirajTekst(obj.GetNaziv());
		_tip = obj.GetTip();
		_pocetak = obj.GetPocetak();
		_trajanjeDana = obj.GetTrajanjeDana();
	}
	~Aranzman() {
		delete[] _oznaka; _oznaka = nullptr;
		delete[] _naziv; _naziv = nullptr;
	}
	const char* GetOznaka() const { return _oznaka; }
	const char* GetNaziv() const { return _naziv; }
	TipAranzmana GetTip() const { return _tip; }
	DatumVrijeme& GetPocetak() { return _pocetak; }
	const DatumVrijeme& GetPocetak() const { return _pocetak; }
	int GetTrajanjeDana() const { return _trajanjeDana; }
	// ToString vraca podatke u formatu:
	// oznaka | naziv | tip | pocetak | broj dana
	// TA-EK-0042-AB | Obilazak Toskane | EKSKURZIJA | 15.07.2026 08:00 | 5
	string ToString() {
		stringstream ss;

		ss << _oznaka << " | " << _naziv << " | " << TipAranzmanaNazivi[_tip] << " | " << _pocetak << " | " << _trajanjeDana;

		return ss.str();
	}
	// ImaPreklapanjeSa vraca true ako se termini (od pocetka do pocetak+trajanjeDana)
	// dva aranzmana preklapaju. aranzman koji pocinje onog dana kada se prethodni
	// zavrsava nije preklapajuci.
	bool ImaPreklapanjeSa(const Aranzman& armn) {
		int pocetakPrvog = _pocetak.ukupnoDana();
		int pocetakDrugog = armn.GetPocetak().ukupnoDana();

		int krajPrvog = pocetakPrvog + _trajanjeDana;
		int krajDrugog = pocetakDrugog + _trajanjeDana;

		if (krajPrvog <= pocetakDrugog || krajDrugog <= pocetakPrvog) return false;

		return true;
	}
};

class Putnik {
	static int _id;
	char* _sifra;
	char* _imePrezime;
	vector<Aranzman> _aranzmani;
public:
	Putnik(const char* imePrezime = "") {
		_imePrezime = AlocirajTekst(imePrezime);
		_sifra = AlocirajTekst(GenerisiOznaku(imePrezime, _id, ODMOR).c_str());
		_id++;
	}
	Putnik(const Putnik& obj) {
		_sifra = AlocirajTekst(obj.GetSifra());
		_imePrezime = AlocirajTekst(obj.GetImePrezime());
		_aranzmani = obj._aranzmani;
	}
	Putnik& operator=(const Putnik& obj) {
		if (this != &obj)
		{
			delete[] _sifra; _sifra = nullptr;
			delete[] _imePrezime; _imePrezime = nullptr;

			_sifra = AlocirajTekst(obj.GetSifra());
			_imePrezime = AlocirajTekst(obj.GetImePrezime());
			_aranzmani = obj._aranzmani;
		}
		return *this;
	}
	~Putnik() {
		delete[] _sifra; _sifra = nullptr;
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	const char* GetSifra() const { return _sifra; }
	const char* GetImePrezime() const { return _imePrezime; }
	vector<Aranzman>& GetAranzmani() { return _aranzmani; }
	// PrijaviAranzman dodaje aranzman putniku ako se ne preklapa ni sa jednim vec
	// prijavljenim aranzmanom, i ako ukupan broj dana putovanja u kalendarskoj godini
	// (zbirno sa novim aranzmanom) ne prelazi 30 dana. metoda vraca true/false.
	bool PrijaviAranzman(const Aranzman& armn) {
		int ukupanBrojDana = 0;

		for (size_t i = 0; i < _aranzmani.size(); i++)
		{
			if (_aranzmani[i].ImaPreklapanjeSa(armn)) return false;
		}

		for (size_t i = 0; i < _aranzmani.size(); i++)
		{
			ukupanBrojDana += _aranzmani[i].GetTrajanjeDana();
		}
		ukupanBrojDana += armn.GetTrajanjeDana();

		if (ukupanBrojDana > 30) return false;

		_aranzmani.push_back(armn);
		return true;
	}
	friend ostream& operator<<(ostream& COUT, Putnik& obj) {
		COUT << obj._imePrezime << " [" << obj._sifra << "]" << endl;
		for (auto& aranzman : obj._aranzmani)
			COUT << " - " << aranzman.ToString() << endl;
		return COUT;
	}
	bool operator==(const Putnik& obj) const {
		return strcmp(obj.GetSifra(), _sifra) == 0;
	}
};

int Putnik::_id = 1;


class Agencija {
	char* _naziv;
	vector<Putnik> _putnici;
public:
	Agencija(const char* naziv = "") { _naziv = AlocirajTekst(naziv); }
	Agencija(const Agencija& obj) {
		_naziv = AlocirajTekst(obj._naziv);
		_putnici = obj._putnici;
	}
	Agencija& operator=(const Agencija& obj){
		if (this != &obj)
		{
			delete[] _naziv; _naziv = nullptr;

			_naziv = AlocirajTekst(obj._naziv);
			_putnici = obj._putnici;
		}
		return *this;
	}
	~Agencija() { delete[] _naziv; _naziv = nullptr; }
	const char* GetNaziv() const { return _naziv; }
	vector<Putnik>& GetPutnici() { return _putnici; }
	// DodajPutnika onemogucava dodavanje putnika sa istom sifrom i baca izuzetak
	void DodajPutnika(const Putnik& putnik) {
		for (auto& p : _putnici) {
			if (p == putnik) throw exception("Putnik je vec dodan");
		}
		_putnici.push_back(putnik);
	}
	// PronadjiPutnika vraca pokazivac na putnika sa proslijedjenom sifrom.
	// Ako putnik nije pronadjen, metoda vraca nullptr.
	Putnik* PronadjiPutnika(string sifraPutnika) {
		for (size_t i = 0; i < _putnici.size(); i++)
		{
			if (string(_putnici[i].GetSifra()) == sifraPutnika) return &_putnici[i];
		}
		return nullptr;
	}
	// PrijaviAranzmanZaPutnika pronalazi putnika po sifri i pokusava mu prijaviti
	// aranzman koristeci pravila metode PrijaviAranzman. Nakon uspjesne prijave,
	// u zasebnom thread-u, treba poslati obavijest putniku sa razmakom od 2 sekunde,
	// sadrzaja:
	//
	// To: <sifra putnika>@agencija.ba
	// From: info@agencija.ba
	// Subject: Potvrda rezervacije
	// Postovani <ime i prezime>,
	// Vasa rezervacija za aranzman <naziv aranzmana> (<tip>) je potvrdjena.
	// Polazak: <datum i vrijeme pocetka>
	// Prijatno putovanje!
	//
	// Ako putnik ne postoji ili prijava nije uspjela, obavijest se ne salje i
	// metoda vraca false.
	bool PrijaviAranzmanZaPutnika(string sifraPutnika, const Aranzman& armn) {
		Putnik* putnik = PronadjiPutnika(sifraPutnika);
		if (putnik == nullptr) return false;
		if (!putnik->PrijaviAranzman(armn)) return false;

		thread t([&]() {
			this_thread::sleep_for(chrono::seconds(2));

			cout << "To: " << putnik->GetSifra() << "@agencija.ba" << endl;
			cout << "From: info@agencija.ba" << endl;
			cout << "Subject: Potvrda rezervacije" << endl;
			cout << "Postovani " << putnik->GetImePrezime() << "," << endl;
			cout << "Vasa rezervacija za aranzman "
				<< armn.GetNaziv()
				<< " (" << TipAranzmanaNazivi[armn.GetTip()] << ") je potvrdjena." << endl;
			cout << "Polazak: " << armn.GetPocetak() << endl;
			cout << "Prijatno putovanje!" << endl;
			});

		t.join();
	}
	
	// AktivniPutnici vraca pokazivace na putnike koji imaju najmanje onoliko
	// prijavljenih aranzmana koliko je definisano vrijednoscu parametra.
	vector<Putnik*> AktivniPutnici(int minAranzmana) {
		vector<Putnik*> temp;
		for (size_t i = 0; i < _putnici.size(); i++)
		{
			if (_putnici[i].GetAranzmani().size() >= minAranzmana) temp.push_back(&_putnici[i]);
		}
		return temp;
	}
	// StatistikaPoTipu vraca kolekciju koja za svaki tip aranzmana, redoslijedom
	// definisanim enumom TipAranzmana, sadrzi tip i ukupan broj prijavljenih
	// aranzmana tog tipa u agenciji (ukljucujuci tipove sa nula prijava).
	Kolekcija<TipAranzmana, int, 4> StatistikaPoTipu() {
		Kolekcija<TipAranzmana, int, 4> temp;
		for (size_t k = 0; k < 4; k++)
		{
			int brojPrijavljenih = 0;
			for (size_t i = 0; i < _putnici.size(); i++)
			{
				for (size_t j = 0; j < _putnici[i].GetAranzmani().size(); j++)
				{
					Aranzman& a = _putnici[i].GetAranzmani()[j];
					if (a.GetTip() == (TipAranzmana)(k)) brojPrijavljenih++;
				}
			}
			temp.Dodaj((TipAranzmana)k, brojPrijavljenih);
		}
		return temp;
	}
};

int main() {
	/*
	Funkcija GenerisiOznaku generise oznaku u formatu: TA-TP-BBBB-IN
	TA -> fiksna oznaka agencije,
	TP -> oznaka tipa aranzmana: OD (odmor), EK (ekskurzija), AV (avantura), PO (poslovno),
	BBBB -> redni broj popunjen nulama na slobodnim mjestima (raspon 1-9999),
	IN -> inicijali prve i posljednje rijeci imena i prezimena.
	Za neispravne podatke funkcija vraca "TA-XX-0000-XX".
	*/
	if (GenerisiOznaku("Amina Buric", 42, EKSKURZIJA) == "TA-EK-0042-AB")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznaku("Goran Skondric", 7, ODMOR) == "TA-OD-0007-GS")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznaku("Ana Marija Kovac", 156, AVANTURA) == "TA-AV-0156-AK")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznaku("Amina", 42, EKSKURZIJA) == "TA-XX-0000-XX" &&
		GenerisiOznaku("Amina Buric", 0, EKSKURZIJA) == "TA-XX-0000-XX" &&
		GenerisiOznaku("Amina Buric", 10000, EKSKURZIJA) == "TA-XX-0000-XX")
		cout << "Neispravni podaci za oznaku OK" << crt;

	// koristeci regex, funkcija ValidirajOznaku provjerava da li je oznaka
	// zapisana u prethodno definisanom formatu.
	if (ValidirajOznaku("TA-EK-0042-AB"))
		cout << "OZNAKA VALIDNA" << crt;
	if (!ValidirajOznaku("TA-XX-0042-AB") && !ValidirajOznaku("TA-EK-042-AB") &&
		!ValidirajOznaku("TA-EK-0042-Ab") && !ValidirajOznaku("TA-EK-0000-AB"))
		cout << "OZNAKA NIJE VALIDNA" << crt;

	Kolekcija<int, string, 5> brojevi;
	brojevi.Dodaj(10, "Deset");
	brojevi.Dodaj(20, "Dvadeset");
	brojevi.Dodaj(30, "Trideset");
	cout << brojevi << crt;

	Kolekcija<int, string, 5> prosireniBrojevi = brojevi.DodajNaPoziciju(1, 15, "Petnaest");
	cout << prosireniBrojevi << crt;

	pair<int, string> uklonjeni = prosireniBrojevi.UkloniSaPozicije(2);
	cout << "Uklonjeno: " << uklonjeni.first << " " << uklonjeni.second << crt;
	cout << "Preostali elementi:" << crt << prosireniBrojevi;

	try {
		prosireniBrojevi.UkloniSaPozicije(10);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	Kolekcija<int, string, 5> kopijaBrojeva = brojevi;
	kopijaBrojeva[0] = 100;
	Kolekcija<int, string, 5> dodijeljeniBrojevi;
	dodijeljeniBrojevi = brojevi;
	dodijeljeniBrojevi[1] = 200;
	cout << "Original:" << crt << brojevi;
	cout << "Kopija:" << crt << kopijaBrojeva;
	cout << "Dodijeljeni objekat:" << crt << dodijeljeniBrojevi;

	DatumVrijeme polazak1(15, 7, 2026, 8, 0);
	DatumVrijeme polazak2(18, 7, 2026, 9, 0);
	DatumVrijeme polazak3(1, 8, 2026, 7, 0);

	// ToString vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM, ukljucujuci pocetne nule
	cout << polazak1.ToString() << crt; // 15.07.2026 08:00

	if (polazak2 > polazak1)
		cout << "Polazak2 je hronoloski nakon polazak1" << crt;

	DatumVrijeme kopijaPolaska(polazak2);
	if (kopijaPolaska == polazak2 && !(polazak1 == polazak2))
		cout << "Provjera vremena, OK." << crt;

	Aranzman toskana("Obilazak Toskane", "Amina Buric", 42, EKSKURZIJA, polazak1, 5);
	Aranzman planine("Avantura na Alpama", "Goran Skondric", 7, AVANTURA, polazak2, 4);
	Aranzman konferencija("Poslovni summit", "Ana Marija Kovac", 156, POSLOVNO, polazak3, 3);

	// ToString vraca podatke u formatu:
	// oznaka | naziv | tip | pocetak | broj dana
	cout << toskana.ToString() << crt;
	if (toskana.ToString() == "TA-EK-0042-AB | Obilazak Toskane | EKSKURZIJA | 15.07.2026 08:00 | 5")
		cout << "Aranzman ToString OK" << crt;

	if (toskana.ImaPreklapanjeSa(planine))
		cout << "Termini se preklapaju" << crt;
	if (!toskana.ImaPreklapanjeSa(konferencija))
		cout << "Termini se ne preklapaju" << crt;

	Putnik amina("Amina Buric"), goran("Goran Skondric"), ana("Ana Marija Kovac");

	if (amina.PrijaviAranzman(toskana))
		cout << "Aranzman prijavljen" << crt;
	if (!amina.PrijaviAranzman(planine))
		cout << "Aranzman nije prijavljen - preklapanje termina" << crt;

	Agencija centarSarajevo("Centar Sarajevo"), poslovnicaMostar("Poslovnica Mostar");

	centarSarajevo.DodajPutnika(amina);
	centarSarajevo.DodajPutnika(goran);
	poslovnicaMostar.DodajPutnika(ana);

	try {
		centarSarajevo.DodajPutnika(amina);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	string sifraGorana = goran.GetSifra();
	Putnik* pronadjen = centarSarajevo.PronadjiPutnika(sifraGorana);
	if (pronadjen != nullptr)
		cout << "Pronadjen putnik: " << pronadjen->GetImePrezime() << crt;
	if (centarSarajevo.PronadjiPutnika("TA-XX-9999-XX") == nullptr)
		cout << "Nepostojeci putnik nije pronadjen" << crt;

	if (centarSarajevo.PrijaviAranzmanZaPutnika(sifraGorana, planine))
		cout << "Prijava uspjesna, obavijest poslana" << crt;
	if (!centarSarajevo.PrijaviAranzmanZaPutnika("NEPOSTOJECA", konferencija))
		cout << "Obavijest nije poslana za nepostojeceg putnika" << crt;

	vector<Putnik*> aktivni = centarSarajevo.AktivniPutnici(1);
	for (auto putnik : aktivni)
		cout << putnik->GetImePrezime() << " ima " << putnik->GetAranzmani().size() << " aranzmana" << crt;

	Kolekcija<TipAranzmana, int, 4> statistika = centarSarajevo.StatistikaPoTipu();
	for (int i = 0; i < statistika.GetTrenutno(); i++)
		cout << TipAranzmanaNazivi[(int)statistika.GetElement1(i)] << " -> " << statistika.GetElement2(i) << crt;

	Agencija kopijaAgencije = centarSarajevo;
	cout << kopijaAgencije.GetNaziv() << " ima " << kopijaAgencije.GetPutnici().size() << " putnika" << crt;

	cin.get();
	return 0;
}