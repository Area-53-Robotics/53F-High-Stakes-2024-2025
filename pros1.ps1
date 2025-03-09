# File paths
$variablesFile = ".\src\Variable\variables.cpp"
$projectFile = ".\project.pros"

# Configurable auton selector variable
$autonSelectorPattern = 'int auton\s*=\s*-?\d+;'

# Project names with optional custom slots
$projectNames = @{
    1 = @{ Name = "Blue AWP Ladder"; CustomSlot = 1 };
    2 = @{ Name = "Blue AWP RR"; CustomSlot = 2 };
    3 = @{ Name = "Blue GoalRush"; CustomSlot = 3 };
    4 = @{ Name = "Blue Right"; CustomSlot = 4 };
    5 = @{ Name = "Red AWP Ladder"; CustomSlot = 5 };
    6 = @{ Name = "Red AWP RR"; CustomSlot = 6 };
    7 = @{ Name = "Red GoalRush"; CustomSlot = 7 };
    8 = @{ Name = "Red Left"; CustomSlot = 8 };
    9 = @{ Name = "Skills"; CustomSlot = 9 };
}

# Predefined auton groups
$autonKeywords = @{
    "red" = @(5, 6, 7, 8, 9);
    "blue" = @(1, 2, 3, 4, 9);
    "both" = @(1, 2, 3, 4, 5, 6, 7, 8);
    "clear" = @();
}

# Exit script on errors
$ErrorActionPreference = "Stop"

# Function to update variables.cpp
function Update-VariablesCpp {
    param ([int]$AutonValue)

    if (!(Test-Path $variablesFile)) {
        Write-Host "Error: variables.cpp not found!" -ForegroundColor Red
        exit 1
    }

    try {
        $content = Get-Content $variablesFile -Raw
        $newContent = $content -replace $autonSelectorPattern, "int auton = $AutonValue;"

        if ($content -ne $newContent) {
            $newContent | Set-Content $variablesFile
            Write-Host "Updated variables.cpp: Set auton to $AutonValue"
        } else {
            Write-Host "Warning: No change detected in variables.cpp (auton may already be set)" -ForegroundColor Yellow
        }
    } catch {
        Write-Host "Error: Failed to update variables.cpp" -ForegroundColor Red
        exit 1
    }
}

# Function to update project.pros
function Update-ProjectPros {
    param ([int]$SlotNumber, [string]$ProjectName)

    if (!(Test-Path $projectFile)) {
        Write-Host "Error: project.pros not found!" -ForegroundColor Red
        exit 1
    }

    try {
        $content = Get-Content $projectFile
        $content = $content -replace '"project_name": ".*",', "`"project_name"": ""$ProjectName`","
        $content = $content -replace 'slot = \d+', "slot = $SlotNumber"
        $content | Set-Content $projectFile
        Write-Host "Updated project.pros: Set slot to $SlotNumber and project_name to '$ProjectName'"
    } catch {
        Write-Host "Error: Failed to update project.pros" -ForegroundColor Red
        exit 1
    }
}

# Function to upload code
function Upload-Code {
    param ([int]$SlotNumber)

    Write-Host "Uploading code to slot $SlotNumber..."
    $output = pros mu --slot $SlotNumber 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Error: PROS upload failed!" -ForegroundColor Red
        Write-Host $output
        exit 1
    }
}

# Main logic
if ($args.Count -eq 0) {
    Write-Host "Error: No input provided. Please enter a number, keyword, or auton name." -ForegroundColor Red
    exit 1
}

$input = $args[0]
$specifiedSlot = $args -contains "--slot" ? ($args[$args.IndexOf("--slot") + 1] -as [int]) : $null

if ($projectNames.ContainsKey($input)) {
    # Number-based auton
    $project = $projectNames[$input]
    $autonValue = $input
    $projectName = $project["Name"]
    $uploadSlot = $specifiedSlot -ne $null ? $specifiedSlot : ($project["CustomSlot"] -ne $null ? $project["CustomSlot"] : 1)

    Update-VariablesCpp -AutonValue $autonValue
    Start-Sleep -Milliseconds 500
    Update-ProjectPros -SlotNumber $uploadSlot -ProjectName $projectName
    Start-Sleep -Milliseconds 500
    Upload-Code -SlotNumber $uploadSlot

} elseif ($autonKeywords.ContainsKey($input)) {
    # Group-based auton
    $autonList = $autonKeywords[$input]
    $uploadSlot = 1

    foreach ($auton in $autonList) {
        if ($projectNames.ContainsKey($auton)) {
            $project = $projectNames[$auton]
            Update-VariablesCpp -AutonValue $auton
            Start-Sleep -Milliseconds 500
            Update-ProjectPros -SlotNumber $uploadSlot -ProjectName $project["Name"]
            Start-Sleep -Milliseconds 500
            Upload-Code -SlotNumber $uploadSlot
            Start-Sleep -Milliseconds 500
            $uploadSlot++
        }
    }

    while ($uploadSlot -le 8) {
        Update-VariablesCpp -AutonValue -1
        Start-Sleep -Milliseconds 500
        Update-ProjectPros -SlotNumber $uploadSlot -ProjectName "No Auton"
        Start-Sleep -Milliseconds 500
        Upload-Code -SlotNumber $uploadSlot
        Start-Sleep -Milliseconds 500
        $uploadSlot++
    }

} else {
    Write-Host "Error: Invalid input '$input'. Please enter a valid number, auton name, or keyword." -ForegroundColor Red
    exit 1
}

Write-Host "Done!"
