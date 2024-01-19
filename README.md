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

Client: Tag1 LOGOUT
