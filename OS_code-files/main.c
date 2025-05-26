#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <time.h>

#define KAT_SAYISI 10        // Apartmanda toplam 10 kat
#define DAIRE_SAYISI 4       // Her katta 4 daire

// === Global senkronizasyon kaynakları (ortak kaynaklar) ===
sem_t vinc_sem;             // En fazla 2 daire aynı anda vinç kullanabilir
sem_t usta_sem;             // En fazla 3 daire aynı anda tesisat yapabilir
sem_t iskele_sem;           // En fazla 2 daire aynı anda iskele kullanabilir

// === Saatli yazı fonksiyonu ===
// Her çıktının başına zaman damgası ekler, loglamayı kolaylaştırır
void saatliYaz(const char* format, int daireNo) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char saat[9];
    strftime(saat, sizeof(saat), "%H:%M:%S", t);
    printf("[%s] ", saat);
    printf(format, daireNo);
}

// === Temizlik işlemini gerçekleştiren fonksiyon ===
// Her daire için ayrı bir thread olarak çağrılır, temizlik sürecini simüle eder
void* temizlikYap(void* arg) {
    int daireNo = *(int*)arg;
    saatliYaz(" >> Temizlikçi: Daire %d temizleniyor...\n", daireNo);
    sleep(1);  // Temizlik süresi
    saatliYaz(" >> Temizlikçi: Daire %d temizliği tamamlandı\n", daireNo);
    return NULL;
}

// === Daire bazlı işlemleri gerçekleştiren thread fonksiyonu ===
// Tesisat, sıva, boya, vinç, iskele ve temizlik işlemleri burada sırasıyla yürütülür
void* daireIslemleri(void* arg) {
    int daireNo = *(int*)arg;

    saatliYaz(" Daire %d: İnşaata başlandı\n", daireNo);

    // --- 1. Tesisat işlemi (usta sınırı var) ---
    // Aynı anda sadece 3 daire ustalık hizmeti alabilir
    sem_wait(&usta_sem);  // Usta kaynağına erişim için beklenir
    saatliYaz(" Daire %d: Usta Tesisat Döşüyor...\n", daireNo);
    sleep(3);              // Tesisat süresi
    sem_post(&usta_sem);  // Usta kaynağı serbest bırakılır

    // --- 2. Sıva işlemi ---
    // Paralel olarak yürütülebilir, sınırlama yok
    saatliYaz(" Daire %d: Sıva işlemi yapılıyor...\n", daireNo);
    sleep(2);  // Sıva süresi

    // --- 3. Boya işlemi ---
    saatliYaz(" Daire %d: Boya işlemi yapılıyor...\n", daireNo);
    sleep(2);  // Boyama süresi

    // --- 4. Vinç kullanımı ---
    // Aynı anda en fazla 2 daire vinç kullanabilir
    sem_wait(&vinc_sem);  // Vinç kaynağına erişim için beklenir
    if (rand() % 100 < 15) {
        // Vinç arızası senaryosu (rastgele hata)
        saatliYaz(" Daire %d: Vinç bozuldu, tamir bekleniyor...\n", daireNo);
        sleep(3);  // Tamir süresi
    }
    saatliYaz(" Daire %d: Vinç kullanılıyor (cam montajı)...\n", daireNo);
    sleep(2);  // Vinç kullanım süresi
    sem_post(&vinc_sem);  // Vinç kaynağı serbest bırakılır

    // --- 5. İskele kullanımı ---
    // Aynı anda 2 daire iskele kullanabilir
    sem_wait(&iskele_sem);  // İskeleye erişim için beklenir
    saatliYaz(" Daire %d: İskele kullanılıyor (dış cephe işlemi)...\n", daireNo);
    sleep(1);
    sem_post(&iskele_sem);  // İskele kaynağı serbest bırakılır

    // --- 6. Temizlik işlemi (ayrı thread ile çağrılır) ---
    pthread_t temizlikci;
    pthread_create(&temizlikci, NULL, temizlikYap, &daireNo);
    pthread_join(temizlikci, NULL);  // Temizlik tamamlanana kadar beklenir

    // --- 7. İnşaat tamamlandı bildirimi ---
    saatliYaz(" Daire %d: İnşaat tamamlandı\n", daireNo);
    return NULL;
}

int main() {
    srand(time(NULL));  // Rastgele olaylar için başlangıç değeri

    // === Temel Atma Aşaması ===
    printf(">>> Apartman projesine başlanmadan önce temel atılıyor...\n");
    sleep(2);  // Temel atma süresi
    printf(">>> Temel atma tamamlandı. Kat inşaatlarına başlanıyor...\n\n");

    // === Senkronizasyon kaynaklarını başlat ===
    sem_init(&vinc_sem, 0, 2);     // 2 vinç aynı anda kullanılabilir
    sem_init(&usta_sem, 0, 3);     // 3 usta aynı anda çalışabilir
    sem_init(&iskele_sem, 0, 2);   // 2 iskele aynı anda kullanılabilir

    // === Katlar sırayla başlatılıyor ===
    for (int kat = 1; kat <= KAT_SAYISI; kat++) {
        pid_t pid = fork();  // Her kat için ayrı bir process başlatılır

        if (pid == 0) {
            // === Çocuk process (bir katın işlemleri) ===
            printf("\n\n===> Kat %d inşaatı başladı (PID: %d)\n\n", kat, getpid());

            pthread_t daireler[DAIRE_SAYISI];
            int daireNumaralari[DAIRE_SAYISI];

            // O kata ait 4 daire için thread başlat
            for (int i = 0; i < DAIRE_SAYISI; i++) {
                daireNumaralari[i] = (kat - 1) * DAIRE_SAYISI + i + 1;
                pthread_create(&daireler[i], NULL, daireIslemleri, &daireNumaralari[i]);
            }

            // Tüm daire işlemleri bitene kadar bekle
            for (int i = 0; i < DAIRE_SAYISI; i++) {
                pthread_join(daireler[i], NULL);
            }

            // Kat tamamlandığında çıktı ver ve child process sonlandır
            printf("\n===> Kat %d inşaatı tamamlandı\n", kat);
            printf("\n----------------------------------------------------------------------\n");
            exit(0);
        } else {
            // === Ana process: sıradaki kata geçmeden önce bekler ===
            wait(NULL);  // Önceki kat tamamlanmadan sonraki başlatılmaz
        }
    }

    // === Semafor kaynaklarını serbest bırak ===
    sem_destroy(&vinc_sem);
    sem_destroy(&usta_sem);
    sem_destroy(&iskele_sem);

    // === Final çıktısı ===
    printf("\n------------- Tüm apartman inşaatı başarıyla tamamlandı -------------\n");
    return 0;
}
