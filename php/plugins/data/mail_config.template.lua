options.timeout = 120
options.subscribe = true

account = IMAP {
    server = 'server',
    username = 'username',
    password = 'password',
    ssl = 'tls1',
}

results = account.INBOX:is_unseen()

print(#results)