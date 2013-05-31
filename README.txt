Unklarheiten:
    Wer beginnt das Spiel?
    lt. Protokoll Abschnitt 3.2 Punkt 1 "Der Initiator ermittelt zuf ̈
    allig den Spieler R mit dem ersten Spielzug."
    Es wird jedoch nirgends spezifiziert, wann und wie er das dem Client mitteilt.
    ANNAHME: der Initiator beginnt IMMER das Spiel

    Wozu gibt es eine Spielfeld ID? Wozu ein synchronize_game_board
    Dafür hat das Protokoll doch Parameter x/y/z...
    Um einen Spielzug zu übermitteln reich EIN Spielzug

    synchronize_game_board
    Da der Scala-Host anstatt eines moves, ein synchronize_game_board sendet um mir
    seinen Zug mitzuteilen werde ich dies auch als move verarbeiten.

    Angabe: "* gegen Maschine"-Modi
    Beachten Sie, dass bei den Modi * gegen Maschine immer gegen die KI auf einem anderen
    System gespielt werden muss, dies ist also nur durch die Auswahl eines Spiels, dass bereits
    im Laufen ist möglich (dadurch wird automatisch ein Computergegner erstellt).
    --> Wenn ein Spiel bereits am laufen ist, kann man ihm nicht mehr beitreten. Es ist
        nicht möglich das umzusetzen


    Angabe Architektur
    Die Architektur der Software besteht im Wesentlichen aus Clients und einem oder mehreren
    Indizierungsservern. In Clients werden beliebig viele Spiele gehostet. Diese werden
    anschließend über Indizierungsserver bekannt und zugänglich gemacht. Es können beliebig
    viele Spielprozesse pro Host gestartet werden, jeder Spielprozess kann beliebig viele Spiele
    zur Verfügung stellen (wenn ein menschlicher Spieler im Spiel ist und ein Client sich zu dem
    gehosteten Spiel connected wird automatisch ein Computerspieler erstellt).


    Das kann doch nicht wirklich so gemeint sein!?
        client |---0< spiele
        host   |---0< spielprozesse |---0< spiele

    --> so wie das da steht kann gar kein Mensch spielen, da immer ein Computergegner erstellt wird

    Heartbeat
    Von einem Ausfall können Indizierungsserver und Clients betroffen sein. Folgendes ist zu beachten:
    • Solange ein Initiator ein Spiel auf einem Indizierungsserver I registiert hat, muss I innerhalb von 60
    Sekunden mindestens ein Heartbeat (siehe Abschnitt 4.4.6) vom Initiator empfangen. Das gleiche
    gilt mit vertauschten Rollen.

    Bei Empfang eines Heartbeats wird das Timeout neu gesetzt. Stellt I
    ein Timeout fest, müssen alle Spiele des Initiators entfernt werden. Um UDP inh ̈
    arente Paketverluste
    zu kompensieren, wird empfohlen, alle 10 Sekunden ein Heartbeat zu senden.

    • Stellt sich bei einem Spielzustandsupdate an den Indizierungsserver heraus, dass dieser nicht mehr
    erreichbar ist, werden alle Spieler u
     ber das vorzeitige Spielende informiert.

    • Stellt der Initiator ein Timeout von 10 Sekunden zum Gegenspieler fest, so wird das Spiel abgebro-
     chen und alle Spieler und der Indizierungsserver u
     ber das vorzeitige Spielende informiert.




Das Projekt wird bei einem persönlichen Abschlussprojekt präsentiert. Dabei sind
mindestens folgende Teile zu präsentieren:
· Konzeptueller Entwurf (Skizzen)
· Klassendiagramm vor Implementierung
· Fertiges Klassendiagramm
· Alle „Spezialinhalte“ zur Notenverbesserung (siehe nächster Punkt)


java -cp lib/scala-library.jar:lib/scala-actors.jar:lib/4gw.jar connect4.network.server.IndexServer 9090
java -cp lib/scala-library.jar:lib/scala-actors.jar:lib/4gw.jar connect4.network.server.GameServer game#1 4 4 1 127.0.0.1 8080 127.0.0.1 9090

Antonius 4 Gewinnt:
https://launchpad.net/~antoniusriha/+archive/con4
