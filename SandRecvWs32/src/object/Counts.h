#pragma once

#include <gl/GL.h>
#include <unordered_set>

class ObjectCounts {
public:

    static const std::unordered_set<GLsizei>& GetPlayerCounts() {
        static const std::unordered_set<GLsizei> playerCounts = {
            15432, 15186, 18198, 16218, 14136, 14004, 
            17544, 16944, 8292, 16491, 29130, 32724, 35112, 22170, 32484, 32400,
            25044, 25797, 23835, 43584, 36756, 25176, 30450, 30846, 
            21138, 32262, 37728, 43083, 34089, 57834, 73656, 69486, 29028,
            32607, 61398, 30972, 26346, 47232, 50592, 32214, 42909, 
            60063, 32277, 32253, 48840, 25848, 59424, 65982, 56748, 41586,
            31236,
            8016, 23691, 22584, 34719, 40209, 29109, 19656, 48012,
            16656, 35445, 25812, 29379, 33720, 42003, 24477, 33732,
            29379, 45732, 18054, 33690,
            14718, 21330, 25758, 18132, 20277, 24771, 25038, 27534,
            32934, 20808, 25356, 29709,
            48930, 58368, 52116, 35886, 22584, 40287, 36534, 34431,
            55167, 33558, 31080, 39777, 40161, 27450, 59433, 54072,
            52608, 22632, 52470, 48435, 5364, 60777, 48579, 32400,
            40995, 32874, 34800, 5123, 
            56430, 58245, 53673, 51066, 79590, 16393, 32715, 30780,
            65535, 32937, 32874, 40996, 66396, 38628, 48579, 47040,
            54552, 44418, 24147, 90996, 9258, 36543, 40287, 30012, 51234, 16392, 32811, 52290, 39768, 60201, 65385, 88128, 61716, 57876, 11871

        };
        return playerCounts;
    }


    static const std::unordered_set<GLsizei>& GetEntityCounts() {
        static const std::unordered_set<GLsizei> entityCounts = {
            28428, // Снорк
            24516, // Слепой пес
            19038, // Тушкан
            72114, // Псевдособака
            38058, // Плоть
            37410, // Кровосос
            23202, // Кабан
            72114, // Пси-собака
            42246, // Химера
            64734  // Псевдогигант 
        };
        return entityCounts;
    }

    static const std::unordered_set<GLsizei>& GetCaseCounts() {
        static const std::unordered_set<GLsizei> CaseCounts = { 11367, 5679, 2832 };
        return CaseCounts;
    }
    static const std::unordered_set<GLsizei>& GetTerrainCounts() {
        static const std::unordered_set<GLsizei> terrainCounts = []() {
            std::unordered_set<GLsizei> set;
            for (GLsizei i = 80; i <= 90000; ++i) {
                set.insert(i);
            }
            return set;
            }();
        return terrainCounts;
    }
    static const std::unordered_set<GLsizei>& GetInfectedPlayerCounts() {
        static const std::unordered_set<GLsizei> InfectedPlayerCounts = { 69156 };
        return InfectedPlayerCounts;
    }

    static const std::unordered_set<GLsizei>& GetBagsCounts() {
        static const std::unordered_set<GLsizei> BagsCounts = { 5412, 1140 };
        return BagsCounts;
    }
};
