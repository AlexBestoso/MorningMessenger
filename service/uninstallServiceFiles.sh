#!/usr/bin/bash

sudo service morningService stop
sudo rm /lib/systemd/system/morningService.service
sudo rm /etc/init.d/morningService
sudo rm -rf /var/morningService
sudo systemctl daemon-reload
