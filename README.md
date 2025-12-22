# **TableTime by xeNYARO (a.k.a anonyaro) ver. 1.0.x**  
# **An open-source game inspired by TableTime for playing with friends guess which words were placed based on the context meaning.**
# **Before installation or compilation, ensure you review the: [License](https://github.com/anonyaro/TableTime/blob/master/LICENSE)**

https://github.com/user-attachments/assets/604f823c-d1eb-4854-bc54-21e1b2b0c521

# **Download and Usage Manual**  
1. **Download:** Navigate to the **Releases** section and download the binary corresponding to your OS architecture (x64 for 64-bit or x32 for 32-bit) along with the database (DB) archive. Extract the archive before launching. You can also manually select the database file within the application's settings menu.
2. **Profiling Synchronization:** This feature refers to visual vertical synchronization (VSync).  
3. **Bug Reports:** If you encounter any issues, please report them via the project's **Issues** tab or contact me directly by clicking my profile in the "About" menu.

# [**Support the project if you are interested**](https://www.donationalerts.com/r/xenyaro)
  <a href="https://www.donationalerts.com/r/xenyaro">
    <img 
      src="https://github.com/user-attachments/assets/c89418ad-5a77-4cca-961f-bd8613ad0466"
      width="300" height="200"
      alt="logo"
      style="display:block;"
    />
  </a>
  
# **Technical Stack & Features**
| Technology | Description |
| :--- | :--- |
| **Programming Language** | **C++17/20 (Native)** — High performance and granular memory management |
| **Interface (API)** | **WINAPI** — Direct interaction with the Windows OS for minimal overhead |
| **GDI/GDI+ Rendering** | **Custom HDC Drawing** — Entirely bespoke rendering of all UI components and controls |
| **Privacy & Security** | **Anti-Screen Capture** — Advanced window protection against capture software (OBS, Discord, Zoom, etc.) |

# **Minimum Requirements**   
| Component              | Requirement                                                  |
| ---------------------- | ----------------------------------------------------------- |
| **Processor**          | 1 GHz or faster with PAE, NX, and SSE2 support              |
| **RAM**                | 1 GB (32-bit) / 2 GB (64-bit)                               |
| **Storage**            | 16 GB (32-bit) / 20 GB (64-bit) available space             |
| **Graphics**           | Microsoft DirectX 9 capable device with WDDM driver         |
| **Display**            | Full HD (1920×1080) @ 60Hz or higher                        |
| **OS**                 | Windows 10 (any edition)                                    |

# **Recommended Requirements**   
| Component              | Recommendation                                                                  |
| ---------------------- | ------------------------------------------------------------------------------- |
| **Processor**          | 4+ Cores, 2.0 GHz+, Intel Core i5/i7 (10th Gen+) or AMD Ryzen 3000+             |
| **RAM**                | 8–16 GB DDR4                                                                    |
| **Storage**            | SSD with 128 GB or more                                                         |
| **Graphics**           | DirectX 12 / WDDM 2.1+, minimum 2 GB VRAM                                       |
| **Display**            | Full HD (1920×1080) @ 60Hz or higher                        |
| **OS**                 | Windows 11 (any edition)                                                        |

# **Key Functionality**
**1. Privacy Toggling:** Hide or show the window from capture software using hotkey combinations.  
**2. Dynamic Card Management:** Real-time manipulation and control of interface cards.  
**3. Performance Monitoring:** Integrated real-time profiling synchronization and resource tracking.  
**4. Advanced Configuration:** Extended settings for fine-tuning resource consumption monitoring.  
**5. Database Flexibility:** Option to select and connect custom Repositories/Databases in settings.  
**6. Adaptive VSync:** Visual vertical synchronization based on the monitor's refresh rate (Hz).  
**7. Custom UI Elements:** Proprietary control elements designed for a unique user experience.  
**8. Memory Optimization:** Proactive memory clearing to maintain peak performance.  
**9. Runtime Alignment:** Real-time recalibration and alignment of UI controls upon restart.  
**10. Real-time Rendering:** Dynamic drawing of all interface elements in real-time.  

# **Patch Notes:**  
**1. Anti-Capture Automation:** Implemented automated logic for hiding/showing the screen from third-party capture tools (2025.05.28).  
**2. Game Initialization:** The "Start Game/Restart" button now automatically triggers window stealth mode.  
**3. Visibility Toggle:** The card-hide button ("♣ ♥ ♦") restores window visibility to capture software.
