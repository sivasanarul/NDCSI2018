/*
NumPro: Finite Elements for Research and Teaching
Copyright (C) 2013 Institut fuer Baustatik und Baudynamik
                   Universitaet Stuttgart
                   Malte von Scheven
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
Contact:
Dr.-Ing. Malte von Scheven
Institut fuer Baustatik und Baudynamik
Universitaet Stuttgart
Paffenwaldring 7
70550 Stuttgart, Germany
http://www.ibb.uni-stuttgart.de/
mvs@ibb.uni-stuttgart.de
*/



#ifndef LIST_H_
#define LIST_H_

#include "Main.h"

/** Template-Klasse zur Beschreibung von Listen
 *
 */
template <class V>
class List
{

private:
  int                             size;               ///< Laenge der Liste
  int                             next;               ///< Index des naechsten freien Eintrags
  V*                              data;               ///< Zeiger auf den Inhalt der Liste

  bool                            ordered;            ///< Flag ob der Inhalt sortiert ist


public:


  /** Konstruktor fuer eine Liste der Laenge Null
   *
   */
  List ( )
  {
    size       = 0;
    next       = 0;
    ordered    = false;
    data       = NULL;
  }




  /** Konstruktor fuer eine Liste mit gegebener Laenge
   *  Die Daten werden NICHT initialisiert!!
   *  Alle Eintraege sind unbenutzt, d.h. next = 0
   *
   */
  List (
      unsigned                    s                   ///< Laenge der Liste
      )
  {
    size       = s;
    next       = 0;
    ordered    = false;
    data       = (V*) new V[s];
  }




  /** Kopier-Konstruktor
   *  Erzeugt eine Kopie einer Liste, der Inhalt wird einzeln kopiert.
   *
   */
  List (
      List const&                 l2                  ///< Vorlage fuer die Kopie
      )
  {
    size      = l2.get_size();
    next      = l2.get_next();
    ordered   = l2.get_ordered();

    data      = (V*) new V[size];

    for(int r=0; r<size; r++)
      data[r] = l2[r];
  }




  /** Destruktor
   *  Inhalt wird geloescht.
   *
   */
  ~List ()
  {
    if (size != 0)
      delete [] data;
  }




  /** Zugriffs-Operator
   *  Gibt den Inhalt einer Liste fuer einen gegebenen Index zurueck.
   *  Es wird geprueft, ob die Laenge der Liste eingehalten wird.
   *  Wurde auf einen Eintrag zugegriffen, gilt er als benutzt,
   *  next wird auf den naechsten Eintrag gesetzt.
   *
   */
  inline V &operator[](
      int const&                  index               ///< Index des gewuenschten Eintrags
      )
  {
    if (index > size - 1) {
      cerr << "Laufzeitfehler: Listenindex " << index << " ungueltig!" << endl;
      exit (EXIT_FAILURE);
    }

    if (index >= next)
      next = index+1;

    return data [ index ];
  }




  /** Zugriffs-Operator
   *  Gibt den Inhalt einer Liste fuer einen gegebenen Index zurueck.
   *  Es wird geprueft, ob die Laenge der Liste eingehalten wird.
   *  Wurde auf einen Eintrag zugegriffen, gilt er als benutzt,
   *  next wird auf den naechsten Eintrag gesetzt.
   *
   */
  inline V &operator[](
      int const&                  index               ///< Index des gewuenschten Eintrags
      ) const
  {
    if (index > size - 1) {
      cerr << "Laufzeitfehler: Listenindex " << index << " ungueltig!" << endl;
      exit (EXIT_FAILURE);
    }

    /*
    if (index >= next)
      next = index+1;
    */

    return data [ index ];
  }




  /** Zuweisungs-Operator
   *
   */
  List<V> operator= (
      List<V> const&              l2                  ///< rechte Seite fuer die Zuweisung
      )
  {
    if (this == &l2)
      return *this;

    delete[] data;
    data   = 0;

    size        = l2.get_size();
    next        = l2.get_next();
    ordered     = l2.get_ordered();

    data        = (V*) new  V[size];

    for(int r=0; r<size; r++)
      data[r] = l2[r];

    return *this;
  }




  /** Laenge der Liste veraendern
   *  Veraendert die Laenge einer Liste, dabei wird der Inhalt erhalten.
   *  Wird die Liste verkuerzt, kann Inhalt verloren gehen.
   *
   */
  void resize (
      int                         s                   ///< neue Laenge der Liste
      )
  {
    /* Fallunterscheidung
     * 1. Liste war vorher leer
     * 2. Groesse bleibt unveraendert s == size
     * 3. Liste wird laenger oder kuerzer
     */

    if (next == 0)
    {
      // Fall 1: Liste war leer
      //========================

      // alten Inhalt loeschen
      delete[] data;
      data        = 0;

      // neuen Inhalt anlegen
      size        = s;
      data        = (V*) new V[s];

      return;
    }
    else if ( s == size )
    {
      // Fall 2: Groesse bleibt unveraendert
      //=====================================

      // nichts zu tun!!
      return;
    }
    else if ( s != size )
    {
      // Fall 3: Liste wird laenger oder kuerzer
      //=========================================


      // alten Inhalt abspeichern
      V* old_data = (V*) new V[next];
      for(int r=0; r<next; r++)
        old_data[r] = data[r];


      // alten Inhalt loeschen
      delete[] data;
      data      = 0;


      // neuen Inhalt anlegen
      size        = s;
      data        = (V*) new V[s];
      if (next > s)
      {
        next        = s;
      }


      // restore old data
      for(int r=0; r<next; r++)
        data[r] = old_data[r];

      delete[] old_data;
      return;
    }

  }




  /** Laenge der Liste veraendern
   *  Unbenutzte Eintraege der Liste werden geloescht, d.h. die Liste
   *  wird auf die benutzte Laenge verkuerzt.
   *
   */
  void resize ()
  {
    /* Fallunterscheidung
     * 1. Liste war vorher leer
     * 2. Liste wird kuerzer
     */

    if (next == 0)
    {
      // Fall 1: Liste war leer,
      // d.h. Groesse wird auf 0 reduziert
      //===================================

      // alten Inhalt loeschen
      delete[] data;
      data        = 0;

      // keinen neuen Inhalt anlegen
      size        = 0;

      return;
    }
    else
    {
      // Fall 2: Liste wird kuerzer
      //============================


      // alten Inhalt abspeichern
      V* old_data = (V*) new V[next];
      for(int r=0; r<next; r++)
        old_data[r] = data[r];


      // alten Inhalt loeschen
      delete[] data;
      data      = 0;


      // neuen Inhalt anlegen
      size        = next;
      data        = (V*) new V[next];


      // restore old data
      for(int r=0; r<next; r++)
        data[r] = old_data[r];

      delete[] old_data;
      return;
    }

  }




  /** Leert eine Liste
   *  Es werden alle Eintraege zu Null (0) und auf unbenutzt gesetzt,
   *  d.h. next = 0.
   *
   */
  inline void reset ()
  {
    for(int r=0; r<size; r++)
      (*this)[r] = 0;

    next = 0;
    return;
  }




  /** Leert eine Liste
   *  Es werden alle Eintraege zum gegebenen Wert und auf unbenutzt gesetzt,
   *  d.h. next = 0.
   *
   */
  inline void reset (
      V                      val                 ///< vorgegebener Wert (i)
  )
  {
    for(int r=0; r<size; r++)
      (*this)[r] = val;

    next = 0;
    return;
  }




  /** neuen Eintrag an eine Liste anhaengen
   *  Gibt es noch freie Eintraege, wird der naechste Freie (next) verwendet,
   *  ist die Liste bereits voll wird die Laenge um eins vergroessert.
   *
   */
  inline void append(
      V const&                    val                 ///< neue Eintrag
      )
  {
    if (next < size)
    {
      // es ist noch Platz in der Liste
      data[next] = val;
      next++;
    }
    else if (next == size)
    {
      // Liste ist voll!!
      (*this).resize(size+1);
      data[next] = val;
      next++;
    }
    else
    {
      cout << "Interner Fehler in List!!";
      exit(1);
    }

    return;
  }




  /** Sucht einen Eintrag in der Liste
   *  Sucht einen gegebenen Wert in der Liste und gibt den Index zurueck.
   *  Ist die Liste sortiert (ordered == true) wird eine Bisektions-Verfahren
   *  verwendet, fuer unsortierte Listen wird jeder Eintrag von Vorne nach Hinten geprueft.
   *
   */
  inline int find(
      V const&                    val                 ///< gesuchter Wert
      ) const
  {
    int index = -1;

    if (ordered == true)
    {
      // Bisektions-Verfahren

      // urspruengliche Grenzen der Suche setzen
      int lower = 0;
      int upper = next-1;

      // solange Suchgebiet halbieren, bis Groesse des Suchgebiets 0 ist
      do
      {
        int mitte = (lower+upper)/2;
        if (data[mitte] == val)
        {
          index = mitte;
          break;
        }
        else if (data[mitte] > val)
          // val is in lower part
          upper = mitte;
        else
          lower = mitte;
      }
      while (upper != lower);
    }
    else
    {
      for(int i=0; i<next; i++)
      {
        if (data[i] == val)
        {
          index = i;
          break;
        }
      }
    }

    return index;
  }




  /** Ausgabe der Liste auf den Bildschirm
   *
   */
  inline void print () const
  {
    printf("\nListe %d(%d)\n", next-1, size);
    for (int r=0; r<next; r++)
      printf("%16.8e\n",(*this)[r]);
    printf("\n");
    return;
  }




  /** Ausgabe der Liste auf den Bildschirm
   *
   */
  void print (
      char*                       name                ///< Bezeichnung der Liste
      ) const
  {
    printf("\nListe %s: %d(%d)\n", name, next-1, size);
    for (int r=0; r<next; r++)
      printf("%16.8e\n",(*this)[r]);
    printf("\n");
    return;
  }




  /** Rueckgabe der Laenge der Liste
   *
   */
  int get_size() const
  {
    return size;
  }




  /** Rueckgabe des Index des naechsten freien Eintrags
   *
   */
  int get_next() const
  {
    return next;
  }




  /** Rueckgabe des Flags ordered
   *
   */
  bool get_ordered() const
  {
    return ordered;
  }




  /** Setzen des Flags ordered
   *
   */
  void set_ordered(
      bool                        o                   ///< neuer Wert fuer ordered
      )
  {
    ordered = o;
    return;
  }


};


#endif /* LIST_H_ */
