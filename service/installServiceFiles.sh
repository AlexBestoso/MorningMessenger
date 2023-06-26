#!/usr/bin/bash

# setup working directory
sudo mkdir /var/morningService
sudo mv ./morningservice.out /var/morningService/morningservice.out

# Setup .service file.
cp ./morningService.template ./morningService.service
ESCAPED_REPLACE=$(printf '%s\n' "/var/morningService" | sed -e 's/[\/&]/\\&/g')
REPLACE="s/WorkingDirectory\=.*\$/WorkingDirectory\=$ESCAPED_REPLACE/g"
sed "$REPLACE" morningService.template > morningService.service

ESCAPED_REPLACE=$(printf '%s\n' "$(which bash)" | sed -e 's/[\/&]/\\&/g')
REPLACE="s/ExecStart\=.*\$/ExecStart\\=$ESCAPED_REPLACE\\ \\/etc\\/init.d\\/morningService start/g"
sed -i "$REPLACE" morningService.service

ESCAPED_REPLACE=$(printf '%s\n' "$(which bash)" | sed -e 's/[\/&]/\\&/g')
REPLACE="s/ExecStop\=.*\$/ExecStop\\=$ESCAPED_REPLACE\\ \\/etc\\/init.d\\/morningService stop/g"
sed -i "$REPLACE" morningService.service

sudo mv ./morningService.service /lib/systemd/system/morningService.service

# Setup init.d service.
cp ./morningService ./morningService.tmp
#ESCAPED_REPLACE=$(printf '%s\n' "$(pwd)" | sed -e 's/[\/&]/\\&/g')
#REPLACE="s/PATH\=.*/PATH\=\"$ESCAPED_REPLACE\"/g"
#sed -i "$REPLACE" ./morningService.tmp

ESCAPED_REPLACE=$(printf '%s\n' "$(which start-stop-daemon)" | sed -e 's/[\/&]/\\&/g')
REPLACE="s/start-stop-daemon/$ESCAPED_REPLACE/g"
sed -i "$REPLACE" ./morningService.tmp


ESCAPED_REPLACE=$(printf '%s\n' "$(which sudo)" | sed -e 's/[\/&]/\\&/g')
REPLACE="s/sudo/$ESCAPED_REPLACE/g"
sed -i "$REPLACE" ./morningService.tmp

sudo mv ./morningService.tmp /etc/init.d/morningService

# update permissions
sudo chown -R MorningNoLogin:MorningNoLogin /var/morningService

# reload daemon cache
sudo systemctl daemon-reload
