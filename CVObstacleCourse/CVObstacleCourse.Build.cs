// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class CVObstacleCourse : ModuleRules
{
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
    }

    public CVObstacleCourse(ReadOnlyTargetRules Target) : base(Target)
    {
        // set up all of the project dependencies as you cannot just set them in your IDE for Unreal engine projects
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI", "RenderCore" });
        
        LoadOpenCV(Target);
        
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PublicDependencyModuleNames.AddRange(new string[] { "RHI", "RenderCore", "Media", "MediaAssets" });
    }

    public bool LoadOpenCV(ReadOnlyTargetRules Target)
    {
        // Start OpenCV linking here!
        bool bIsLibrarySupported = false;

        // Create OpenCV Path 
        string OpenCVPath = Path.Combine(ThirdPartyPath, "OpenCV");
        
        // the OpenCV library will ony work on windows64 platform, this will stop it from attempting to build anything else
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            bIsLibrarySupported = true;
        }
        else
        {
            string Err = string.Format("{0} dedicated server is made to depend on {1}. We want to avoid this, please correct module dependencies.", Target.Platform.ToString(), this.ToString()); System.Console.WriteLine(Err);
        }

        if (bIsLibrarySupported)
        {
            //Add Include path 
            PublicIncludePaths.AddRange(new string[] { Path.Combine(OpenCVPath, "Includes") });

            //Add Static Libraries
            //Open CV Libraries
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "OpenCV/Libraries/Win64/opencv_world320.lib"));
            //My Own DLL Libraries
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "OpenCV/Libraries/Win64/CVObstacleCourseFunctions.lib"));

            //Add Dynamic Libraries
            //Open CV Libraries
            PublicDelayLoadDLLs.Add("opencv_world320.dll");
            PublicDelayLoadDLLs.Add("opencv_ffmpeg320_64.dll");
            //My Own DLL Libraries
            PublicDelayLoadDLLs.Add("CVObstacleCourseFunctions.dll");
        }

        PublicDefinitions.Add(string.Format("WITH_OPENCV_BINDING={0}", bIsLibrarySupported ? 1 : 0));

        return bIsLibrarySupported;
    }
}
