> [!NOTE]
> **About this fork**
>
> The upstream project is no longer maintained, and with a current Steam client it
> could no longer pair: the TV showed "Unknown error" after entering the PIN. I made
> this fork to get IHSplay working again on my LG webOS TV.
>
> What is different from [mariotaku/ihsplay](https://github.com/mariotaku/ihsplay):
>
> - **Pairing works with current Steam.** Steam now pairs devices through a
>   PIN-masked key exchange and rejects the old method. The library (`core`) points
>   to [tijavo/ihslib](https://github.com/tijavo/ihslib), which is based on
>   [cnapolit/ihslib](https://github.com/cnapolit/ihslib), where the new pairing was
>   implemented. IHSplay stores the key negotiated with each host, so you only pair
>   once.
> - **No more frozen picture or missing sound.** A single lost network packet could
>   stall the video or audio stream until a lot of new data arrived, so an idle screen
>   stayed frozen. Stale packets are now dropped after 200 ms.
> - **Magic Remote pointer.** On webOS the remote's pointer stays visible while
>   streaming, and clicks go where the pointer is.
> - **Clearer errors.** A rejected pairing says so instead of "Unknown error".
> - **Debug log on webOS.** Create an empty file named `debug` next to the `ihsplay`
>   executable, and the app writes its log to `ihsplay.log` in the same folder. Apps
>   started from the TV menu have no other visible output.
>
> **Streaming the whole desktop from a Linux host on Wayland:** Steam can only capture
> the game or Big Picture window by default. When you switch to another window, the
> picture freezes or the connection closes. Start Steam with `steam -pipewire` and
> confirm the screen share dialog on the host.

```
    ______  _______       __
   /  _/ / / / ___/____  / /___ ___  __      In-Home
   / // /_/ /\__ \/ __ \/ / __ `/ / / /     Streaming
 _/ // __  /___/ / /_/ / / /_/ / /_/ /     Player
/___/_/ /_//____/ .___/_/\__,_/\__, /     For Steam
               /_/            /____/
```

IHSplay is an SDL2 based Steam Link Client, primarily targeting webOS TV
and Raspberry Pi.

The application will be providing a TV remote / gamepad friendly UI, and
low latency video / audio output, as well as generic mouse / keyboard /
gamepad support.

---

![Home UI](https://user-images.githubusercontent.com/830358/219081008-e5eb646a-76fa-4683-9869-35e3923618ce.png)

![Streaming Overlay](https://user-images.githubusercontent.com/830358/207047480-8cc96496-4fab-4a0e-8d1a-896183c54a6e.png)


Stay tuned!
