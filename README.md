Proiect Email Server      ~ Thunderbird Client
    Smtp Workflow:
    Serverul are incontinuu pe listen un socket pe portul 25.
    Clientul trimite un mail catre server printr-un socket prin portul 25.
    La deschiderea socket-ului conexiune:
    S: 220 \r\n - Acesta este un greet dat de server cand se deschide conexiunea
    C: EHLO [domain] 
    S: 250 OK \r\n - Codul de raspuns 250 este ca un "OK", spunandu-i clientului ca comanda executa a fost buna si poate trece la urmatoarea.
    C: MAIL FROM:<sender@domain>
    S: 250 OK \r\n - In caz ca comada este OK.
    C: RCPT TO:<recipient@domain>
    S: 250 OK \r\n
    C: DATA \r\n
    S: "354 OK \r\n" - Aici serverul este pregatit sa primeasca continut mail-ului efectiv.
    C:
    C:  "Aici se afla continutul mesajului efectiv"
    C:
    C: . - Acesta arata ca clientul a terminat de trimis continutul.
    S: 250 OK\r\n
    Imap Workflow:
Client : Tag1 LOGIN Username Password
Server : * OK [CAPABILITY IMAP4rev1 AUTH=PLAIN ]
         Tag1 OK CAPABILITY completed

Client : Tag2 AUTHENTICATE PLAIN <base64-encoded-authentication-information>
Server : Tag2 OK AUTHENTICATE completed

Client : A1 LIST "" "*"
Server : * LIST (\HasNoChildren) "/" "inbox"
         * LIST (\HasNoChildren) "/" "drafts"
         * LIST (\HasNoChildren) "/" "sent"
         * LIST (\HasNoChildren) "/" "trash"
         A1 OK LIST completed
         
Client : Tag1 Select [Gmail]/All Mail
Server : * FLAGS (#Answered #Flags #Draft #Deleted #Seen $Forworded)
         * OK [PERMANENTFLAGS (#Answered #Flagged #Draft #Deleted #Seen $Forworded *)] Flags permited.
         * OK [UIDVALIDITY 594586685] UIDs valid.
         * 322 EXISTS
         * 0 RECENT
         * OK [UIDNEXT 813] Predicted next UID.
         Tag1 OK [READ-WRITE] [Gmail]/All Mail selected. (Success)

Client : Tag1 FETCH 2 (BODY[])
Server : * 4 FETCH (BODY[] {700}
         Return-Path: <xxx@gmail.com>
         Received: from ?126.250.27.92? (pv126250027092.10.tss.panda-word.ne.jp [126.250.27.92])
               by my.google.com with ESHTMPS id y3sn549330t ia.3.2009.04.03.07.07.32
               (version=TLSv1/SSLv3 cipher=RC4-MD5);
               FRI, 03 Apr 2009 07:07:34 -0700 (PDF)
          Message-Id: <9E247AAF-3E51-4795-A2C0-3DF3F99D5D76@gmail.com>
          FROM: Xman <xxx@gmail.com>
          TO: Yman <yyy@gmail.com>
          Content-Type: text/plain;
                  Charset=us-ascii;
                  format=flowed
          Content-Transfer-Encoding: 7bit
          X-Mailer: iPhone Mail (5H11)
          Mine-Version: 1.0 (iPhone Mail 5H11)
          Date: Fri,3 Apr 2009 23:06:54 +0900
          Hi, This is test mail
          )
          Tag1 OK Success

Client: Tag1 LOGOUT
