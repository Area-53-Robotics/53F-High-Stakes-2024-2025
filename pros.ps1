# Parameters
param (
    [string]$Keyword,  # Optional keyword for predefined auton lists
    [int]$Slot         # The auton slot number (e.g., 1-19), used if no keyword is given
)

# File paths
$variablesFile = "C:\Users\Admin\Documents\Robotics\53F\src\Variable\variables.cpp"
$projectFile = "C:\Users\Admin\Documents\Robotics\53F\project.pros"

# Project names based on auton slot
$projectNames = @{
    1 = "53F_HS Red Right";
    2 = "53F_HS Red Left";
    3 = "53F_HS Blue Right";
    4 = "53F_HS Blue Left";
    5 = "53F_HS Skills";
    6 = "53F_HS Blue Goal";
    7 = "53F_HS Red Goal";
    8 = "53F_HS WRedLeft";
    9 = "53F_HS WPRedLeft";
}

# Predefined auton lists for specific keywords
$autonKeywords = @{
    "red" = @(1, 2, 7, 8, 9, 5);  # Autons for "red"
}

# Determine the maximum slot number
$maxSlot = $projectNames.Keys | Measure-Object -Maximum | Select-Object -ExpandProperty Maximum

# Function to upload auton code
function Upload-Code {
    param (
        [int]$SlotNumber,    # Upload slot number
        [int]$AutonValue,    # Auton value
        [string]$ProjectName # Project name
    )

    # Update variables.cpp
    (Get-Content $variablesFile) -replace 'int auton = \d+;', "int auton = $AutonValue;" | Set-Content $variablesFile
    Write-Host "Updated variables.cpp: Set auton to $AutonValue"

    # Update project.pros
    $projectContent = Get-Content $projectFile
    $projectContent = $projectContent -replace '"project_name": ".*",', "`"project_name"": ""$ProjectName`","
    $projectContent = $projectContent -replace 'slot = \d+', "slot = $SlotNumber"
    $projectContent | Set-Content $projectFile
    Write-Host "Updated project.pros: Set slot to $SlotNumber and project_name to '$ProjectName'"

    # Upload code to the specified slot
    Write-Host "Uploading code to slot $SlotNumber..."
    pros mu --slot $SlotNumber
}

# Process based on keyword or slot number
if ($Keyword) {
    if ($autonKeywords.ContainsKey($Keyword)) {
        $autonList = $autonKeywords[$Keyword]
        $uploadSlot = 1

        # Upload each auton in the list
        foreach ($auton in $autonList) {
            if ($projectNames.ContainsKey($auton)) {
                Upload-Code -SlotNumber $uploadSlot -AutonValue $auton -ProjectName $projectNames[$auton]
                $uploadSlot++
            }
        }

        # Fill remaining slots with empty values
        while ($uploadSlot -le 8) {
            Upload-Code -SlotNumber $uploadSlot -AutonValue -1 -ProjectName "Empty Slot"
            $uploadSlot++
        }
    } else {
        Write-Host "Error: Unknown keyword '$Keyword'. Available options: $($autonKeywords.Keys -join ', ')." -ForegroundColor Red
        exit 1
    }
} else {
    # Validate the auton slot
    if ($Slot -lt 1 -or $Slot -gt $maxSlot) {
        Write-Host "Error: Invalid auton number '$Slot'. Please enter a valid auton number (1-$maxSlot)." -ForegroundColor Red
        exit 1
    }

    # Default behavior for a single slot
    Upload-Code -SlotNumber $Slot -AutonValue $Slot -ProjectName $projectNames[$Slot]
}

Write-Host "Done!"
