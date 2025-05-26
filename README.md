# 🏗️ OS Construction Model

This project simulates the construction of a **10-story apartment building**, where each floor and apartment represents **processes and threads** in an operating system. It's built using **C language**, utilizing **POSIX processes**, **threads**, and **semaphores** to demonstrate **synchronization**, **parallelism**, and **resource management**.

> 📅 **Course:** Operating Systems – 2024-2025 Spring Semester  
> 👨‍💻 **Team Members:**  
> Aleks Dulda – 21360859025  
> Büşra Uzunlar – 20360859098  
> 💾 **Language:** C (GCC / POSIX)


---


## 📌 Project Description

Each floor is simulated as a **separate process**, while each apartment on a floor is a **thread**. Daire (apartment) threads execute a set of construction tasks with synchronization rules:

- **Thread (Apartment)** tasks:
  - Plumbing (limited to 3 simultaneous jobs)
  - Plastering
  - Painting
  - Crane use (limited to 2 simultaneous)
  - Scaffold use (limited to 2 simultaneous)
  - Cleaning (spawned as a nested thread)

- **Process (Floor)** rules:
  - Floor *n+1* cannot start before *n* is completed (uses `wait()`).
  - All apartments must finish before a floor is marked completed.

---

## 🔧 Features

✅ Multi-process and multi-threaded modeling  
✅ Realistic construction workflow simulation  
✅ Critical section management with semaphores  
✅ Time-stamped logs for monitoring execution  
✅ Random crane failure to simulate unpredictability  
✅ Modular and well-commented C source code  

---

### 📹 Project Demo (Video)

A walkthrough video demonstrating the functionality, structure, and synchronization mechanisms of the project:

👉 [Click here to watch the video on YouTube](https://youtu.be/tx0ytQs2n6o)

> 📌 *This video explains the process-thread model, synchronization with semaphores, and the execution flow with example outputs.*

---

## 🧱 Build & Run

### ▶️ Requirements

- GCC compiler
- POSIX thread and semaphore support (Linux/macOS)

### 🔨 Compile

```bash
gcc -pthread -o construction main.c
```

### 🚀 Run
```bash
./construction
```

---

| Concept                 | Implementation                                         |
| ----------------------- | ------------------------------------------------------ |
| Multi-processing        | Each floor is launched as a child process via `fork()` |
| Multi-threading         | Each apartment is a thread using `pthread_create()`    |
| Synchronization         | Shared semaphores: `vinc`, `usta`, `iskele`            |
| Resource limitation     | Max 3 plumbers, 2 cranes, 2 scaffolders                |
| Process coordination    | `wait()` ensures floors are built sequentially         |
| Race condition handling | Protected access to shared resources                   |
| Nested threads          | Cleaning step is handled in a separate thread          |

---

## 📸 Sample Output
```text
>>> Temel atma tamamlandı. Kat inşaatlarına başlanıyor...

[14:22:10]  Daire 1: İnşaata başlandı
[14:22:10]  Daire 1: Usta Tesisat Döşüyor...
...
[14:22:14]  >> Temizlikçi: Daire 1 temizleniyor...
[14:22:15]  >> Temizlikçi: Daire 1 temizliği tamamlandı
[14:22:15]  Daire 1: İnşaat tamamlandı
```

---

## 📁 Project Structure
```bash
.
├── main.c           # Main simulation source code
├── README.md        # Project documentation
```

---

## 📬 Contact
For questions or contributions:

> **Aleks Dulda**  
> 📧 21360859025@ogr.btu.edu.tr
>  
> **Büşra Uzunlar**  
> 📧 20360859098@ogr.btu.edu.tr

---

> 📝 **Note (TR):** Bu proje, işletim sistemleri dersinde process, thread ve senkronizasyon kavramlarını  
> bir apartman inşaatı üzerinden modellemek amacıyla geliştirilmiştir. Katlar process, daireler thread olarak  
> simüle edilmiştir.
