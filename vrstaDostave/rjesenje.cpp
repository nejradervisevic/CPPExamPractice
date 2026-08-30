#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <thread>
using namespace std;

string crt = "\n-------------------------------------------------------------------------------- - \n";

char* AlocirajTekst(const char* tekst) {
    if (tekst == nullptr) return nullptr;
    size_t velicina = strlen(tekst) + 1;
    char* noviTekst = new char[velicina];
    strcpy_s(noviTekst, velicina, tekst);
    return noviTekst;
}

enum VrstaDostave { STANDARDNA, BRZA, MEDJUNARODNA };
const char* VrstaDostaveOznake[] = { "ST", "BR", "MD" };
const char* VrstaDostaveNazivi[] = { "STANDARDNA", "BRZA", "MEDJUNARODNA" };

enum StatusPosiljke { KREIRANA, PREUZETA, U_TRANZITU, ISPORUCENA, OTKAZANA };
const char* StatusPosiljkeNazivi[] = { "KREIRANA", "PREUZETA", "U TRANZITU", "ISPORUCENA", "OTKAZANA" };

string GenerisiOznaku(const char* imePrezime, int redniBroj, VrstaDostave vrstaDostave) {
    if (imePrezime == nullptr || strlen(imePrezime) < 1) return "BH-XX-0000-XX";
    if (redniBroj < 1 || redniBroj > 9999) return "BH-XX-0000-XX";
    if (vrstaDostave != STANDARDNA && vrstaDostave != BRZA && vrstaDostave != MEDJUNARODNA) return "BH-XX-0000-XX";

    string oznaka;

    oznaka += "BH-";
    if (vrstaDostave == STANDARDNA) oznaka += "ST";
    else if (vrstaDostave == BRZA) oznaka += "BR";
    else if (vrstaDostave == MEDJUNARODNA) oznaka += "MD";
    oznaka += "-";

    if (redniBroj < 10) oznaka += "000";
    else if (redniBroj < 100) oznaka += "00";
    else if (redniBroj < 1000) oznaka += "0";
    oznaka += to_string(redniBroj);
    oznaka += "-";

    oznaka += toupper(imePrezime[0]);
    const char* razmak = strrchr(imePrezime, ' ');
    if (razmak != nullptr && strlen(razmak) > 1) oznaka += toupper(razmak[1]);
    if (razmak == nullptr || strlen(razmak) < 1) return "BH-XX-0000-XX";

    return oznaka;
}
bool ValidirajOznaku(const string& oznaka) {
    return regex_match(oznaka, regex("BH-(ST|BR|MD)-(?!0000)\\d{4}-[A-Z]{2}"));
}

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
        for (size_t i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = new T1{ obj.GetElement1(i) };
            _elementi2[i] = new T2{ obj.GetElement2(i) };
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
            for (size_t i = 0; i < _trenutno; i++)
            {
                _elementi1[i] = new T1{ obj.GetElement1(i) };
                _elementi2[i] = new T2{ obj.GetElement2(i) };
            }
        }
        return *this;
    }
    int GetTrenutno() const { return _trenutno; }
    T1& GetElement1(int indeks) { return *_elementi1[indeks]; }
    T2& GetElement2(int indeks) { return *_elementi2[indeks]; }
    const T1& GetElement1(int indeks) const { return *_elementi1[indeks]; }
    const T2& GetElement2(int indeks) const { return *_elementi2[indeks]; }
    T1& operator[](int indeks) { return *_elementi1[indeks]; }

    friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
        for (int i = 0; i < obj.GetTrenutno(); i++)
            COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
        return COUT;
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
    void Dodaj(const T1& el1, const T2& el2) {
        if (_trenutno >= max) throw exception("Kolekcija je puna");

        _elementi1[_trenutno] = new T1(el1);
        _elementi2[_trenutno] = new T2(el2);

        _trenutno++;
    }
    Kolekcija DodajNaPoziciju(int pozicija, const T1& el1, const T2& el2) {
        if (_trenutno >= max) throw exception("Kolekcija je puna");
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
    pair<T1, T2> UkloniSaPozicije(int pozicija) {
        if (pozicija < 0 || pozicija > _trenutno) throw exception("Pozicija je izvan opsega");
        pair<T1, T2> temp;
        
        temp = pair<T1, T2>(*_elementi1[pozicija], *_elementi2[pozicija]);

        for (size_t i = pozicija; i < _trenutno - 1; i++)
        {
            _elementi1[i] = _elementi1[i + 1];
            _elementi2[i] = _elementi2[i + 1];
        }

        _trenutno--;
        return temp;
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
            delete _dan;
            delete _mjesec;
            delete _godina;
            delete _sati;
            delete _minute;
            _dan = _mjesec = _godina = _sati = _minute = nullptr;
        }
        return *this;
    }
    friend ostream& operator<<(ostream& COUT, const DatumVrijeme& obj) {
         COUT << obj.ToString();
        return COUT;
    }

    ~DatumVrijeme() {
        delete _dan;
        delete _mjesec;
        delete _godina;
        delete _sati;
        delete _minute;
        _dan = _mjesec = _godina = _sati = _minute = nullptr;
    }

    string ToString() const {
        stringstream ss;

        ss << setw(2) << setfill('0') << *_dan << "."
            << setw(2) << setfill('0') << *_mjesec << "."
            << setw(2) << setfill('0') << *_godina << " "
            << setw(2) << setfill('0') << *_sati << ":"
            << setw(2) << setfill('0') << *_minute;

        return ss.str();
    }

    bool operator>(const DatumVrijeme& obj)const {
        if (*_godina > *obj._godina) return true;
        if (*_godina < *obj._godina) return false;

        if (*_mjesec > *obj._mjesec) return true;
        if (*_mjesec < *obj._mjesec) return false;

        if (*_dan > *obj._dan) return true;
        if (*_dan < *obj._dan) return false;

        if (*_sati > *obj._sati) return true;
        if (*_sati < *obj._sati) return false;

        if (*_minute > *obj._minute) return true;
        if (*_minute < *obj._minute) return false;
    }

    bool operator==(const DatumVrijeme& obj) const {
        return *_dan == *obj._dan && *_mjesec == *obj._mjesec && *_godina == *obj._godina && *_sati == *obj._sati && *_minute == *obj._minute;
    }
};

class Posiljka {
    char* _oznaka;
    char* _sadrzaj;
    char* _imePrezimePrimaoca;
    char* _emailPrimaoca;
    VrstaDostave _vrstaDostave;
    int _redniBroj;
    Kolekcija<StatusPosiljke, DatumVrijeme, 10> _statusi;

    void Dealociraj() {
        delete[] _oznaka;
        delete[] _sadrzaj;
        delete[] _imePrezimePrimaoca;
        delete[] _emailPrimaoca;
        _oznaka = _sadrzaj = _imePrezimePrimaoca = _emailPrimaoca = nullptr;
    }
public:
    Posiljka(const char* sadrzaj, const char* imePrezime, const char* email, VrstaDostave vrsta, int redniBroj, const DatumVrijeme& statusKreiranja) {
        _sadrzaj = AlocirajTekst(sadrzaj);
        _imePrezimePrimaoca = AlocirajTekst(imePrezime);
        _emailPrimaoca = AlocirajTekst(email);
        _vrstaDostave = vrsta;
        _redniBroj = redniBroj;

        _oznaka = AlocirajTekst(GenerisiOznaku(imePrezime, redniBroj, vrsta).c_str());
        _statusi.Dodaj(KREIRANA, statusKreiranja);
    }
    Posiljka(const Posiljka& obj) {
        _sadrzaj = AlocirajTekst(obj.GetSadrzaj());
        _imePrezimePrimaoca = AlocirajTekst(obj.GetImePrezimePrimaoca());
        _emailPrimaoca = AlocirajTekst(obj.GetEmailPrimaoca());
        _vrstaDostave = obj.GetVrstaDostave();
        _redniBroj = obj.GetRedniBroj();
        _statusi = obj.GetStatusi();
        _oznaka = AlocirajTekst(obj.GetOznaka());
    }
    Posiljka& operator=(const Posiljka& obj) {
        if (this != &obj)
        {
            delete[] _oznaka;
            delete[] _sadrzaj;
            delete[] _imePrezimePrimaoca;
            delete[] _emailPrimaoca;
            _oznaka = _sadrzaj = _imePrezimePrimaoca = _emailPrimaoca = nullptr;

            _sadrzaj = AlocirajTekst(obj.GetSadrzaj());
            _imePrezimePrimaoca = AlocirajTekst(obj.GetImePrezimePrimaoca());
            _emailPrimaoca = AlocirajTekst(obj.GetEmailPrimaoca());
            _vrstaDostave = obj.GetVrstaDostave();
            _redniBroj = obj.GetRedniBroj();
            _statusi = obj.GetStatusi();
            _oznaka = AlocirajTekst(obj.GetOznaka());
        }
        return *this;
    }
    const char* GetOznaka() const { return _oznaka; }
    const char* GetSadrzaj() const { return _sadrzaj; }
    const char* GetImePrezimePrimaoca() const { return _imePrezimePrimaoca; }
    const char* GetEmailPrimaoca() const { return _emailPrimaoca; }
    VrstaDostave GetVrstaDostave() const { return _vrstaDostave; }
    int GetRedniBroj() const { return _redniBroj; }
    Kolekcija<StatusPosiljke, DatumVrijeme, 10>& GetStatusi() { return _statusi; }
    const Kolekcija<StatusPosiljke, DatumVrijeme, 10>& GetStatusi() const { return _statusi; }
    StatusPosiljke GetTrenutniStatus() const { return _statusi.GetElement1(_statusi.GetTrenutno() - 1); }

    friend ostream& operator<<(ostream& COUT, const Posiljka& obj) {
        // ToString vraca podatke u formatu:
        // oznaka | ime i prezime | vrsta dostave | sadrzaj | trenutni status
        // BH-BR-0042-AB | Amina Buric | BRZA | Dokumenti | KREIRANA
         COUT << obj.ToString();
        return COUT;
    }

    string ToString() const {
        stringstream ss;

        ss << _oznaka << " | " << _imePrezimePrimaoca << " | " << VrstaDostaveNazivi[_vrstaDostave] << " | " << _sadrzaj << " | " << StatusPosiljkeNazivi[GetTrenutniStatus()];

        return ss.str();
    }

    ~Posiljka() {
        Dealociraj();
    }

    /*
   DodajStatus dodaje status samo ako je vrijeme vece od vremena
   posljednjeg statusa i ako je promjena statusa dozvoljena.

   Dozvoljen slijed statusa je: KREIRANA -> PREUZETA -> U_TRANZITU -> ISPORUCENA

   OTKAZANA se moze evidentirati iz bilo kojeg statusa koji nije zavrsni.
   Nakon statusa ISPORUCENA ili OTKAZANA nisu dozvoljene nove promjene.
   Preskakanje, ponavljanje statusa i hronoloski neispravno vrijeme vracaju
   false, bez izmjene kolekcije statusa.
   */
    bool DodajStatus(StatusPosiljke noviStatus, const DatumVrijeme& vrijemeStatusa) {
        DatumVrijeme posljednjiStatus = _statusi.GetElement2(_statusi.GetTrenutno() - 1);
        if (!(vrijemeStatusa > posljednjiStatus)) return false;

        StatusPosiljke trenutniStatus = _statusi.GetElement1(_statusi.GetTrenutno() - 1);
        if (trenutniStatus == KREIRANA) {
            if (noviStatus != PREUZETA && noviStatus != OTKAZANA) return false;
        }
        if (trenutniStatus == PREUZETA) {
            if (noviStatus != U_TRANZITU && noviStatus != OTKAZANA) return false;
        }
        if (trenutniStatus == U_TRANZITU) {
            if (noviStatus != ISPORUCENA && noviStatus != OTKAZANA) return false;
        }

        if (trenutniStatus == ISPORUCENA || trenutniStatus == OTKAZANA) return false;

        _statusi.Dodaj(noviStatus, vrijemeStatusa);
        return true;
    }

    bool operator==(const Posiljka& obj) const {
        return strcmp(_oznaka, obj.GetOznaka()) == 0 && _redniBroj == obj.GetRedniBroj();
    }
};

class DostavnaSluzba {
    char* _naziv;
    vector<Posiljka> _posiljke;
public:
    DostavnaSluzba(const char* naziv = "") {
        _naziv = AlocirajTekst(naziv);
    }
    DostavnaSluzba(const DostavnaSluzba& obj) {
        _naziv = AlocirajTekst(obj.GetNaziv());
        _posiljke = obj._posiljke;
    }
    DostavnaSluzba& operator=(const DostavnaSluzba& obj) {
        if (this != &obj)
        {
            delete[] _naziv;
            _naziv = nullptr;

            _naziv = AlocirajTekst(obj.GetNaziv());
            _posiljke = obj._posiljke;
        }
    }
    const char* GetNaziv() const { return _naziv; }
    vector<Posiljka>& GetPosiljke() { return _posiljke; }
    ~DostavnaSluzba() {
        delete[] _naziv;
        _naziv = nullptr;
    }

    bool DodajPosiljku(const Posiljka& posiljka) {
        for (size_t i = 0; i < _posiljke.size(); i++)
        {
            if (posiljka == _posiljke[i]) throw exception("Posiljka vec postoji");
        }
        _posiljke.push_back(posiljka);
    }

    /*
   PronadjiPosiljku vraca pokazivac na posiljku sa proslijedjenomnoznakom.
   Ako posiljka nije pronadjena, metoda vraca nullptr.
   */
    Posiljka* PronadjiPosiljku(const string& oznaka) {
        for (size_t i = 0; i < _posiljke.size(); i++)
        {
            if (string(_posiljke[i].GetOznaka()) == oznaka) return &_posiljke[i];
        }
        return nullptr;
    }
    /*
   EvidentirajStatus pronalazi posiljku na osnovu oznake i pokusava dodati
   novi status koristeci pravila metode DodajStatus.
   Nakon svakog uspjesno evidentiranog statusa potrebno je u zasebnom
   threadu
   poslati obavijest primaocu.Obavijest se ne salje ako posiljka ne
   postoji ili status nije dodan, te u tim slucajevima metoda vraca
   false.
    Primjer sadrzaja obavijesti :
    ---------------------------------------------------------------------------------
    To : goran@fit.ba
    From : info@brzaposta.ba
    Subject : Promjena statusa posiljke
    Postovani Goran Skondric,
    Posiljka BH - ST - 0007 - GS je promijenila status u PREUZETA.
    Vrijeme evidentiranja : 01.07.2026 09 : 00
    Hvala sto koristite nase usluge.
    BrzaPosta Tim
    ---------------------------------------------------------------------------------
   */
    bool EvidentirajStatus(const string& oznaka, StatusPosiljke status, const DatumVrijeme& vrijeme) {
        Posiljka* posiljka = PronadjiPosiljku(oznaka);
        if (posiljka == nullptr) return false;
        if (!posiljka->DodajStatus(status, vrijeme)) return false;

        thread t([&]() {
            cout << "-----------------------------------------------------" << endl;
            cout << "To: " << posiljka->GetEmailPrimaoca() << endl;
            cout << "From: info@brzaposta.ba" << endl;
            cout << "Subject: Promjena statusa posiljke" << endl;
            cout << "Postovani " << posiljka->GetImePrezimePrimaoca() << ", \n Posiljka " << posiljka->GetOznaka() << " je promjenila status u " << StatusPosiljkeNazivi[status] << "." << endl;
            cout << "Vrijeme evidentiranja: " << vrijeme.ToString() << endl;
            cout << "Hvala sto koristite nase usluge. \n BrzaPosta Tim" << endl;
            });
        t.join();

        return true;
    }

    vector<Posiljka*> PosiljkePoStatusu(StatusPosiljke trazeniStatus) {
        vector<Posiljka*> temp;
        for (size_t i = 0; i < _posiljke.size(); i++)
        {
            if (_posiljke[i].GetTrenutniStatus() == trazeniStatus) temp.push_back(&_posiljke[i]);
        }
        return temp;
    }

    /*
   StatistikaPoStatusu vraca kolekciju koja za svaki status,
   redoslijedom definisanim enumom StatusPosiljke, sadrzi status i broj posiljki
   koje se trenutno nalaze u tom statusu. U rezultat se dodaje svih pet statusa,
   ukljucujuci i one za koje je broj posiljki jednak nuli.
   */
    Kolekcija<StatusPosiljke, int, 5> StatistikaPoStatusu() {
        Kolekcija<StatusPosiljke, int, 5> temp;
        for (size_t i = 0; i < 5; i++)
        {
            int brojac = 0;
            StatusPosiljke status;
            switch (i)
            {
            case 0: status = KREIRANA; break;
            case 1: status = PREUZETA; break;
            case 2: status = U_TRANZITU; break;
            case 3: status = ISPORUCENA; break;
            case 4: status = OTKAZANA; break;
            }
            for (size_t j = 0; j < _posiljke.size(); j++)
            {
                if (_posiljke[j].GetTrenutniStatus() == status) brojac++;
            }

            temp.Dodaj(status, brojac);
        }
        return temp;
    }
};


int main() {
    /*
    Funkcija GenerisiOznaku generise oznaku posiljke u formatu: BH-VD-BBBB-IN
    Potpis funkcije treba biti:
    string GenerisiOznaku(const char* imePrezime, int redniBroj, VrstaDostave vrstaDostave)

    BH -> fiksna oznaka drzave,
    VD -> vrsta dostave: ST (standardna), BR (brza), MD (medjunarodna),
    BBBB -> redni broj posiljke popunjen nulama na slobodnim mjestima,
    IN -> inicijali imena i prezimena primaoca.
    Kod imena koje sadrzi vise rijeci koriste se inicijal prve i posljednje rijeci.
    Redni broj mora biti u rasponu 1-9999.
    Za neispravne podatke funkcija vraca "BH-XX-0000-XX".
    */

    if (GenerisiOznaku("Amina Buric", 42, BRZA) == "BH-BR-0042-AB")
        cout << "Oznaka OK" << crt;
    if (GenerisiOznaku("Goran Skondric", 7, STANDARDNA) == "BH-ST-0007-GS")
        cout << "Oznaka OK" << crt;
    if (GenerisiOznaku("Ana Marija Kovac", 156, MEDJUNARODNA) == "BH-MD-0156-AK")
        cout << "Oznaka OK" << crt;
    if (GenerisiOznaku("Amina", 42, BRZA) == "BH-XX-0000-XX" &&
        GenerisiOznaku("Amina Buric", 0, BRZA) == "BH-XX-0000-XX" &&
        GenerisiOznaku("Amina Buric", 10000, BRZA) == "BH-XX-0000-XX" &&
        GenerisiOznaku("Amina Buric", 42, (VrstaDostave)99) == "BH-XX-0000-XX")
        cout << "Neispravni podaci za oznaku OK" << crt;

    /*
    Koristeci regex, funkcija ValidirajOznaku provjerava da li je oznaka
    zapisana u prethodno definisanom formatu. Dozvoljene su iskljucivo oznake
    vrsta ST, BR i MD, redni broj mora imati cetiri cifre i ne moze biti 0000,
    a inicijali moraju biti zapisani velikim slovima.
    Potpis funkcije treba biti:
    bool ValidirajOznaku(const string& oznaka)
    */
    if (ValidirajOznaku("BH-BR-0042-AB"))
        cout << "OZNAKA VALIDNA" << crt;
    if (!ValidirajOznaku("BH-EX-0042-AB") &&
        !ValidirajOznaku("BH-BR-042-AB") &&
        !ValidirajOznaku("BH-BR-0042-Ab") &&
        !ValidirajOznaku("BH-BR-0000-AB") &&
        !ValidirajOznaku("bh-BR-0042-AB"))
        cout << "OZNAKA NIJE VALIDNA" << crt;
    if (ValidirajOznaku(GenerisiOznaku("Maid Ramic", 284, BRZA)))
        cout << "GENERISANA OZNAKA VALIDNA" << crt;

    Kolekcija<int, string, 5> brojevi;
    brojevi.Dodaj(10, "Deset");
    brojevi.Dodaj(20, "Dvadeset");
    brojevi.Dodaj(30, "Trideset");
    cout << brojevi << crt;

    /*
    DodajNaPoziciju dodaje novi par na lokaciju/indeks definisanu prvim parametrom,
    pomjera postojece pokazivace udesno i vraca trenutno stanje
    kolekcije (izmijenjeni tj. prosireni objekat).
    U slucaju popunjene kolekcije ili neispravne lokacije potrebno je
    baciti izuzetak.
    */

    Kolekcija<int, string, 5> prosireniBrojevi = brojevi.DodajNaPoziciju(1, 15, "Petnaest");
    cout << prosireniBrojevi << crt;

    /*
    UkloniSaPozicije uklanja par sa proslijedjene pozicije / indeksa, preostale elemente
    pomjera ulijevo i vraca uklonjeni par u obliku pair<T1, T2>.
    Za poziciju izvan opsega baca se izuzetak.
    */
    pair<int, string> uklonjeni = prosireniBrojevi.UkloniSaPozicije(2);
    cout << "Uklonjeno: " << uklonjeni.first << " " << uklonjeni.second << crt;
    cout << "Preostali elementi:" << crt << prosireniBrojevi;

    try {
        prosireniBrojevi.UkloniSaPozicije(10);
    }
    catch (exception& e) {
        cout << "Exception: " << e.what() << crt;
    }

    try {
        Kolekcija<int, string, 3> punaKolekcija;
        punaKolekcija.Dodaj(1, "Jedan");
        punaKolekcija.Dodaj(2, "Dva");
        punaKolekcija.Dodaj(3, "Tri");
        punaKolekcija.DodajNaPoziciju(1, 4, "Cetiri");
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

    DatumVrijeme kreirana(1, 7, 2026, 8, 0);
    DatumVrijeme preuzeta(1, 7, 2026, 9, 0);
    DatumVrijeme tranzit(1, 7, 2026, 10, 0);
    DatumVrijeme isporucena(1, 7, 2026, 11, 0);

    /*
    ToString vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM,
    ukljucujuci pocetne nule.
    */

    cout << kreirana.ToString() << crt; // 01.07.2026 08:00

    if (preuzeta > kreirana)
        cout << "Vrijeme preuzimanja je nakon vremena kreiranja" << crt;

    DatumVrijeme kopijaPreuzeta(preuzeta);
    if (kopijaPreuzeta == preuzeta && !(kreirana == preuzeta))
        cout << "Provjera vremena, OK." << crt;

    /*
    Konstruktor Posiljke generise oznaku i evidentira pocetni status KREIRANA
    sa proslijedjenim vremenom kreiranja.
    */

    Posiljka dokumenti("Dokumenti", "Amina Buric", "amina@fit.ba", BRZA, 42, kreirana);
    Posiljka knjige("Knjige", "Goran Skondric", "goran@fit.ba", STANDARDNA, 7, kreirana);
    Posiljka oprema("Oprema", "Ana Marija Kovac", "ana@fit.ba", MEDJUNARODNA, 156, kreirana);

    /*
    ToString vraca podatke u formatu :
    Oznaka | ime i prezime | vrsta dostave | sadrzaj | trenutni status
    BH-BR-0042-AB | Amina Buric | BRZA | Dokumenti | KREIRANA
    */
    cout << dokumenti.ToString() << crt;
    if (dokumenti.ToString() == "BH-BR-0042-AB | Amina Buric | BRZA | Dokumenti | KREIRANA")
        cout << "Posiljka ToString OK" << crt;

    /*
    DodajStatus dodaje status samo ako je vrijeme vece od vremena
    posljednjeg statusa i ako je promjena statusa dozvoljena.

    Dozvoljen slijed statusa je: KREIRANA -> PREUZETA -> U_TRANZITU -> ISPORUCENA

    OTKAZANA se moze evidentirati iz bilo kojeg statusa koji nije zavrsni.
    Nakon statusa ISPORUCENA ili OTKAZANA nisu dozvoljene nove promjene.
    Preskakanje, ponavljanje statusa i hronoloski neispravno vrijeme vracaju
    false, bez izmjene kolekcije statusa.
    */

    if (dokumenti.DodajStatus(PREUZETA, preuzeta))
        cout << "Status PREUZETA dodan" << crt;
    if (dokumenti.DodajStatus(U_TRANZITU, tranzit))
        cout << "Status U TRANZITU dodan" << crt;
    if (dokumenti.DodajStatus(ISPORUCENA, isporucena))
        cout << "Status ISPORUCENA dodan" << crt;
    if (!dokumenti.DodajStatus(OTKAZANA, isporucena))
        cout << "Promjena zavrsnog statusa nije dozvoljena" << crt;
    if (!knjige.DodajStatus(U_TRANZITU, preuzeta))
        cout << "Preskakanje statusa nije dozvoljeno" << crt;
    if (oprema.DodajStatus(OTKAZANA, preuzeta))
        cout << "Posiljka otkazana" << crt;

    Posiljka kopijaDokumenata = dokumenti;
    cout << kopijaDokumenata << crt;

    DostavnaSluzba brzaPosta("Brza posta");

    /*
    DodajPosiljku dodaje posiljku u dostavnu sluzbu. Nije dozvoljeno dodati
    dvije posiljke sa istim rednim brojem ili istom oznakom. U slucaju
    duplikata metoda baca izuzetak.
    */

    brzaPosta.DodajPosiljku(knjige);
    brzaPosta.DodajPosiljku(oprema);

    try {
        brzaPosta.DodajPosiljku(knjige);
    }
    catch (exception& e) {
        cout << "Exception: " << e.what() << crt;
    }

    /*
    PronadjiPosiljku vraca pokazivac na posiljku sa proslijedjenomnoznakom.
    Ako posiljka nije pronadjena, metoda vraca nullptr.
    */

    string oznakaKnjige = knjige.GetOznaka();
    Posiljka* pronadjena = brzaPosta.PronadjiPosiljku(oznakaKnjige);

    if (pronadjena != nullptr)
        cout << "Pronadjena posiljka: " << pronadjena->GetOznaka() << crt;

    if (brzaPosta.PronadjiPosiljku("BH-ST-9999-XX") == nullptr)
        cout << "Nepostojeca posiljka nije pronadjena" << crt;

    /*
    EvidentirajStatus pronalazi posiljku na osnovu oznake i pokusava dodati
    novi status koristeci pravila metode DodajStatus.
    Nakon svakog uspjesno evidentiranog statusa potrebno je u zasebnom
    threadu
    poslati obavijest primaocu.Obavijest se ne salje ako posiljka ne
    postoji ili status nije dodan, te u tim slucajevima metoda vraca
    false.
    Primjer sadrzaja obavijesti :
    ---------------------------------------------------------------------------------
    To : goran@fit.ba
    From : info@brzaposta.ba
    Subject : Promjena statusa posiljke
    Postovani Goran Skondric,
    Posiljka BH - ST - 0007 - GS je promijenila status u PREUZETA.
    Vrijeme evidentiranja : 01.07.2026 09 : 00
    Hvala sto koristite nase usluge.
    BrzaPosta Tim
    ---------------------------------------------------------------------------------
    */

    if (brzaPosta.EvidentirajStatus(oznakaKnjige, PREUZETA, preuzeta))
        cout << "Status evidentiran i obavijest poslana" << crt;
    if (!brzaPosta.EvidentirajStatus(oznakaKnjige, ISPORUCENA, isporucena))
        cout << "Obavijest nije poslana za neispravnu promjenu statusa" << crt;
    if (!brzaPosta.EvidentirajStatus("NEPOSTOJECA", PREUZETA, preuzeta))
        cout << "Obavijest nije poslana za nepostojecu posiljku" << crt;

    /*
    PosiljkePoStatusu vraca vector pokazivaca na sve posiljke ciji trenutni
    status odgovara proslijedjenom statusu.
    */

    vector<Posiljka*> preuzetePosiljke = brzaPosta.PosiljkePoStatusu(PREUZETA);
    for (auto posiljka : preuzetePosiljke)
        cout << posiljka->GetOznaka() << " -> " << StatusPosiljkeNazivi[(int)posiljka->GetTrenutniStatus()] << crt;

    /*
    StatistikaPoStatusu vraca kolekciju koja za svaki status,
    redoslijedom definisanim enumom StatusPosiljke, sadrzi status i broj posiljki
    koje se trenutno nalaze u tom statusu. U rezultat se dodaje svih pet statusa,
    ukljucujuci i one za koje je broj posiljki jednak nuli.
    */

    Kolekcija<StatusPosiljke, int, 5> statistika = brzaPosta.StatistikaPoStatusu();
    for (int i = 0; i < statistika.GetTrenutno(); i++)
        cout << StatusPosiljkeNazivi[(int)statistika.GetElement1(i)] << " -> " << statistika.GetElement2(i) << crt;

    DostavnaSluzba kopijaSluzbe = brzaPosta;
    cout << kopijaSluzbe.GetNaziv() << " ima " << kopijaSluzbe.GetPosiljke().size() << " posiljki" << crt;

    cin.get();
    return 0;
}