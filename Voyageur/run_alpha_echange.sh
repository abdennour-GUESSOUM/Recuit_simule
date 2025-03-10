#!/bin/bash

# Fichier: recuit_simplifie.sh
# Description: Script qui exécute le recuit simulé avec différents paramètres 
# et sauvegarde les coûts optimaux dans un fichier

# Vérification de l'existence du programme
if [ ! -f "recuitVoyageur_echange" ]; then
    echo "Compilation du programme..."
    make
    if [ $? -ne 0 ]; then
        echo "Erreur lors de la compilation. Arrêt du script."
        exit 1
    fi
fi


# Fichier pour sauvegarder les coûts optimaux
COUTS_FILE="couts_optimauxEchangealphamoinsT.txt"
echo "# Coûts optimaux pour différents paramètres du recuit simulé" > "$COUTS_FILE"
echo "# Date: $(date)" >> "$COUTS_FILE"
echo "" >> "$COUTS_FILE"

# Fonction pour exécuter le programme et extraire le coût optimal
run_recuit_and_get_cost() {
    local t_init=$1
    local t_fin=$2
    local alpha=$3
    local amplitude=$4
    local max_rep=$5
    local param_name=$6
    local param_value=$7
    
    echo "Exécution avec paramètres: Tinit=$t_init, Tfin=$t_fin, alpha=$alpha, amplitude=$amplitude, maxRep=$max_rep"
    
    # Préparation des entrées pour le programme
    cat <<EOF > input_params.txt
$t_init
$t_fin
$alpha
$amplitude
$max_rep
o
EOF
    
    # Exécution du programme avec redirection des sorties
    ./recuitVoyageur_echange < input_params.txt > temp_output.txt 2>&1
    
    # Extraction du coût optimal
    cost=$(grep "Cout optimal" temp_output.txt | grep -o "[0-9]\+\.[0-9]\+" | head -1)
    
    # Sauvegarde du coût dans le fichier
    echo "$param_name=$param_value: $cost" >> "$COUTS_FILE"
    
    # Affichage du coût pour vérification
    echo "  -> Coût optimal: $cost"
    
    # Nettoyage des processus gnuplot qui pourraient rester en arrière-plan
    pkill gnuplot > /dev/null 2>&1
    
    # Pause courte pour s'assurer que tout est bien terminé
    sleep 1
}

# Test alpha avec param opti
echo "== Variation du type ==" | tee -a "$COUTS_FILE"
for t_alpha in 0.85 0.50 0.25 0.1; do
  echo "Test avec t_alpha = $t_alpha" | tee -a "$COUTS_FILE"
  for i in {1..5}; do
    echo " Exécution $i/5" | tee -a "$COUTS_FILE"
    run_recuit_and_get_cost 100 0.01 $t_alpha 1 500 "Talpha" "${t_alpha}_run${i}"
  done
  echo "" >> "$COUTS_FILE"
done
echo "" >> "$COUTS_FILE"

# Nettoyage des fichiers temporaires
rm -f input_params.txt temp_output.txt

echo "Tous les tests sont terminés."
echo "Les coûts optimaux ont été sauvegardés dans le fichier $COUTS_FILE"