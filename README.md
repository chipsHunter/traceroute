## Утилита __traceroute__  

Утилита для диагностики сети. Позволяет отслеживать путь пакета к хосту назначения.   

----------------------------------------
### Принцип работы    

К хосту назначения отсылаются по протоколу UPD пакеты.    
Каждый пакет имеет _время жизни_ TTL (Time To Live) — количество хостов, которое может пройти пакет до того, как быть "отброшенным".     
Хост, получивший пакет с TTL = 0, отсылает назад сообщение по протололу ICMP "Time exceeded".    

----------------------------------------
### Стандартные параметры

* `TTL` = 1 — первый пакет будет "отброшен" на первом хосте    
* `MAX_HOPS` = 30 — максимум 30 хостов может пройти пакет    
* `ATTEMPTS` = 3 — на каждый хост будет отправляться 3 одинаковых пакета     

----------------------------------------
### Флаги утилиты

Стандартные параметры поддежат изменению с помощью этих флагов:

|      Флаг    |   Параметр  |
|:------------:|:------------|
| `-f <число>` | `TTL`       |
| `-q <число>` | `ATTEMPTS`  |   
| `-h <число>` | `MAX_HOPS`  |   

----------------------------------------
### Вывод программы

> [!TIP]
> Утилиту необходимо запускать в виде `sudo hvtraceroute <флаг> <значение> <ваш домен>`
> Использование флагов необязательно, в таком случае будут выставлены стандартные настройки
> Напишите `sudo hvtraceroute --help` для получения помощи о значениях флагов

Так выглядит путь пакетов к [youtube](https://youtube.com)

```bash
 sudo ./traceroute youtube.com
 1 10.66.66.1 202.66 ms * 816.43 ms 
 2 10.66.66.1 0.06 ms 203.68 ms 0.07 ms 
 3 45.13.226.1 202.46 ms 0.07 ms 205.41 ms 
 4 45.145.42.10 0.04 ms 180.65 ms 0.07 ms 
 5 173.194.121.206 125.94 ms * * 
 6 66.249.94.244 518.73 ms 205.11 ms 205.04 ms 
 7 142.250.214.201 204.32 ms 205.61 ms 204.27 ms 
 8 142.250.186.174 204.26 ms 
Reached target: 142.250.186.174
```
--------------------------------------
### Установка

Для установки утилиты достаточно следующей команды в терминале:
```
curl -sSL https://github.com/chipsHunter/traceroute/raw/main/install.sh | bash
```
